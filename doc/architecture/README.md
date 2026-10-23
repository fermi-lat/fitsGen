---
package: fitsGen
summary: Static library for writing FT1 (event) and FT2 (spacecraft) FITS files from templates, plus a merit-tuple reader and an embedded-Python event classifier; its only user is observationSim.
layer: io
depends_on: [tip, astro, facilities, embed_python]
used_by: [observationSim]
last_reviewed_commit: 2b3853c
---

# fitsGen Architecture

**fitsGen** is a small library that creates LAT FT1 (photon event) and FT2
(spacecraft pointing) FITS files from FITS templates and fills them row by
row through `tip`. It was written for the ground pipeline (GlastRelease),
where its `makeFT1`/`makeFT2` tools turned Gleam merit ROOT tuples into FT1
and FT2 files. In the ScienceTools CMake build only the library is built,
and the only user is `observationSim` (`gtobssim`), which uses `Ft1File`
and `Ft2File`.

What is compiled is about 500 lines: `FtFileBase`, `Ft1File`, `Ft2File`,
`MeritFile` and `EventClassifier`. `MeritFile2.cxx` and
`XmlEventClassifier.cxx` compile to empty files (see
[Build and test](#build-and-test)), and the eight programs under `src/*/`
(about 1,200 lines) are not built at all.

`doc/release.notes` stops at fitsGen-07-01-03 (March 2015). Later history
is in git. `doc/FT1_Documentation.html` is an old description of the FT1
columns. There is no `src/mainpage.h`.

## Purpose and scope

**Does:**
- Create an FT1 or FT2 file from a template (`data/ft1.tpl`,
  `ft1_p7.tpl`, `ft2.tpl`, `lle.tpl`), size the table, and hand out a `tip`
  iterator so the caller can set columns row by row.
- Write the time keywords (`TSTART`, `TSTOP`, `DATE-OBS`, `DATE-END`,
  `DATE`, and `TELAPSE` if present) in the data extension, the primary HDU
  and (FT1 only) the `GTI` extension when the file is closed.
- For FT2, compute the attitude quaternion `QSJ_1..4` from the z- and x-axis
  directions (`Ft2File::setScAxes`).
- Read a merit tuple (or any `tip` table) row by row (`MeritFile`).
- Classify events by calling a Python script through `embed_python`
  (`EventClassifier`).

**Does not:**
- Choose what goes in the columns. The caller computes and sets every value.
  The templates define the columns and the fixed keywords.
- Write `GTI` rows, DSS keywords, checksums or `CREATOR`. `observationSim`
  does these itself with `dataSubselector` and `st_facilities`.
- Provide any `gt*` tool. `makeFT1`, `makeFT1_kluge` and `makeFT2` have par
  files in `pfiles/`, and those par files are installed, but the programs
  are not built (see [Invariants and gotchas](#invariants-and-gotchas)).

## Position in the system

| Direction | Package | Why |
|-----------|---------|-----|
| Depends on | `tip` | `PUBLIC`. All FITS creation and editing: `IFileSvc::createFile(out, template)`, `editTable`, `editImage`, `Table::Iterator`. `FtFileBase.h` exposes `tip::Table`, `tip::TableCell` and the iterator in its API. `MeritFile` uses `IFileSvc::readTable`. |
| Depends on | `astro` | `PUBLIC`. `JulianDate` for the date keywords (`FtFileBase.h`), and `SkyDir` and `Quaternion` for `Ft2File::setScAxes`. |
| Depends on | `facilities` | `PUBLIC`. `commonUtilities::getDataPath("fitsGen")` finds the templates; `getPackagePath("fitsGen")` and `setupEnvironment()` are used by `EventClassifier`. |
| Depends on | `embed_python` | `PUBLIC`. `fitsGen/EventClassifier.h` includes `embed_python/Module.h` and holds `PyObject *` members, so every user of fitsGen gets libpython on its link line. See `src/embed_python/doc/architecture/`. |
| Used by | `observationSim` | `Ft1File` (`EventContainer::writeEvents`) and `Ft2File` (`ScDataContainer::writeScData`), linked `PRIVATE`. It does **not** use `EventClassifier`, `MeritFile` or `MeritFile2`, but it still pulls in `embed_python` and libpython through the `PUBLIC` link. |

No other package includes a `fitsGen/` header, and nothing in Python
imports it.

**Tier.** `dependency-layers.md` places fitsGen in `analysis`. Everything it
uses is in `foundation` or `io`, it contains no science algorithms, and it
builds no tools, so this doc puts it in `io` (FITS file writing).

**CMake vs. use.** The `target_link_libraries` list (`facilities tip astro
embed_python`) matches the library's includes. The unbuilt programs also
include `st_app`, `st_stream` and `st_facilities`, which fitsGen does not
link. A build of those programs would need them, but the library does not.

## Key abstractions

```
FtFileBase              owns tip::Table* m_table + Table::Iterator m_it
  ├── Ft1File           EVENTS from ft1.tpl; close() also stamps the GTI HDU
  └── Ft2File           SC_DATA from ft2.tpl; setScAxes() fills RA/DEC_SCZ/SCX + QSJ_1..4

MeritFile               read-only tip table + iterator (merit TTree or FITS table)
EventClassifier         embed_python::Module + Python callable "eventClassifier"
  └── XmlEventClassifier   (ROOT/evtUtils only; compiled out)
MeritFile2              (ROOT TTree reader; compiled out)
```

| Type | Header | Role |
|------|--------|------|
| `FtFileBase` | `fitsGen/FtFileBase.h` | Base for output files. `init(template, extname)` creates the file from the template (or, with an empty template name, appends a new table `extname` to an existing file), opens `extname` for editing, sets the row count and puts the iterator at the first row. `operator[](col)` returns the current row's `tip::TableCell`. `next()`/`prev()`/`itor()`/`begin()`/`end()` walk the rows. `appendField(name, tform)` adds a column. `setObsTimes(start, stop)` stores the MET range written by `close()`. `setPhduKeyword` edits the primary header. `header()` is the data extension's header. Static `setMissionStart`/`missionStart` (default 2001-01-01 00:00) control `DATE-OBS`/`DATE-END`. |
| `Ft1File` | `fitsGen/Ft1File.h` | `Ft1File(outfile, nrows=0, extname="EVENTS", templateFile="ft1.tpl")`. Its `close()` also writes the time keywords into the `GTI` extension, if the template has one. |
| `Ft2File` | `fitsGen/Ft2File.h` | `Ft2File(outfile, nrows=0, extname="SC_DATA", templateFile="ft2.tpl")`. `setScAxes(ra_scz, dec_scz, ra_scx, dec_scx)` sets those four columns and `QSJ_1..4` from `astro::Quaternion(z, x)` for the current row. |
| `MeritFile` | `fitsGen/MeritFile.h` | Wraps `tip::IFileSvc::readTable(file, tree, filter)`. `operator[](col)` returns a `double`. `tstart()`/`tstop()` are `EvtElapsedTime` of the first and last rows. `conversionType()` is 0 (front) if `17 - Tkr1FirstLayer < 11.5`, else 1. It can read any tip table, so the unbuilt `convertFT1` and `partition` used it on FITS `EVENTS` tables. Reading a `.root` merit file needs tip's ROOT support (`FERMI_BUILD_ROOT=ON`, off by default). |
| `EventClassifier` | `fitsGen/EventClassifier.h` | `EventClassifier(moduleName)` imports a Python module through `embed_python::Module` with `$FERMI_INST_DIR/fitsGen/python` (from `getPackagePath`) and `.` on `sys.path`. It reads the module's `meritVariables` list and its `eventClassifier` callable. `operator()` fills a Python dict from a `tip` row or a `std::map<std::string,double>`, calls `eventClassifier(dict)`, and returns the result as `unsigned int`. |
| `XmlEventClassifier`, `MeritFile2` | `fitsGen/XmlEventClassifier.h`, `fitsGen/MeritFile2.h` | ROOT-based versions (`TTree`/`TEventList`, and `evtUtils::EventClass` XML cut definitions). Their `.cxx` files are wrapped in `#ifndef BUILD_WITHOUT_ROOT`, which is always defined here, so they compile to nothing. The headers are still installed. |

### Templates (`data/`, installed)

| File | Extensions | Notes |
|------|------------|-------|
| `ft1.tpl` | primary, `EVENTS` (23 columns `ENERGY` … `DIFRSP4`), `GTI` | `EVENT_CLASS` is `32X` (bit array): the Pass 8 layout. `PASS_VER = 'NONE'`, `NDSKEYS = 0`. |
| `ft1_p7.tpl` | same | Identical except that `EVENT_CLASS` is `I` (16-bit integer), the Pass 7 layout. |
| `ft2.tpl` | primary, `SC_DATA` (31 columns `START` … `SC_VELOCITY`) | `SC_VELOCITY` was added in 2018. |
| `lle.tpl` | primary, `EVENTS` (12 columns), `GTI` | LAT Low Energy event layout. Nothing in the CMake build uses it. |

All templates set `TELESCOP = 'GLAST'`, `INSTRUME = 'LAT'`, `TIMESYS = 'TT'`
and `MJDREFI/MJDREFF = 51910 / 7.428703703703703e-4`.

## Data flow and control flow

How `observationSim` writes an FT1 file (`EventContainer::writeEvents`):

```
fitsGen::Ft1File ft1(name, nrows, "EVENTS", "ft1.tpl" | "ft1_p7.tpl")
  └─ FtFileBase::init
       ├─ template is one of the 4 known names → getDataPath("fitsGen")/<name>
       ├─ tip::IFileSvc::createFile(name, template)
       ├─ editTable(name, "EVENTS"); setNumRecords(nrows)
       └─ m_it = begin()
ft1.appendField("MC_SRC_ID", "1J") ...
for (; ft1.itor() != ft1.end(); ft1.next())  ft1["energy"].set(...) ...
ft1.setObsTimes(tstart, tstop)
ft1.header()[...] / ft1.setPhduKeyword(...)      // caller's own keywords
ft1.close()
  ├─ writeDateKeywords(EVENTS)   DATE, DATE-OBS, DATE-END, TSTART, TSTOP, [TELAPSE]
  ├─ delete table
  ├─ writeDateKeywords(GTI)      (if a GTI HDU exists)
  └─ writeDateKeywords(primary)
```

`Ft2File` follows the same pattern, using `setScAxes` for the attitude
columns. The destructor calls `close()`, so letting the object go out of
scope also finishes the file. Calling `close()` twice is safe because
`m_table` is set to null.

## Public interfaces

### C++ API

Installed headers in `include/fitsGen/`: `FtFileBase.h`, `Ft1File.h`,
`Ft2File.h`, `MeritFile.h`, `EventClassifier.h`, and two unusable ones,
`MeritFile2.h` (includes `TTree.h`) and `XmlEventClassifier.h` (needs
`evtUtils`, which is not in ScienceTools).

Minimal use, as in `observationSim`:

```cpp
#include "fitsGen/Ft2File.h"

fitsGen::Ft2File ft2("sc.fits", nrows);                 // ft2.tpl via getDataPath
for (size_t i = 0; ft2.itor() != ft2.end(); ft2.next(), ++i) {
   ft2["start"].set(start[i]);                          // column names are case-insensitive (tip)
   ft2["stop"].set(stop[i]);
   ft2.setScAxes(raz[i], decz[i], rax[i], decx[i]);     // also fills QSJ_1..4
}
ft2.setObsTimes(start.front(), stop.back());            // else TSTART/TSTOP = -1
ft2.close();
```

### Applications

None built. See the dead-code list under
[Invariants and gotchas](#invariants-and-gotchas).

### Python

No bindings. `python/*.py` are **not installed** by CMake:
- `EventClassifier.py` is the helper base class that turns ROOT TCut strings
  into Python expressions.
- `DC2_Classifier.py`, `Pass4_Classifier.py`, `Pass5_Classifier_old.py`,
  `Pass6_kluge_Classifier.py` and `P6V3_classifier.py` are classifier
  scripts for old passes.
- `Ft1Validator.py` is a standalone FT1 checker.

### Environment variables

fitsGen calls no `getenv` itself. Through `facilities` it depends on
`FERMI_INST_DIR` (templates, classifier path); see "Runtime environment" in
ScienceTools' `docs/architecture/README.md`.

### File formats

- **Writes:** FT1 (`EVENTS` + `GTI`), FT2 (`SC_DATA`), LLE, all shaped by
  the templates above.
- **Reads:** merit tuples or FITS tables through `tip` (`MeritFile`).

## Build and test

- **CMake target:** `fitsGen`, a **STATIC** library built with PIC.
  Compile definitions: `BUILD_WITHOUT_ROOT` (also inherited `PUBLIC` from
  `tip`, so every consumer sees it too) and `TRAP_FPE` (not on macOS).
- **Install:**
  - headers to `include/fitsGen`
  - `pfiles/` (the par files of the unbuilt tools) to `syspfiles`
  - `ft1.tpl`, `ft1_p7.tpl`, `ft2.tpl`, `lle.tpl`, `xml_test_merit.root`
    and `EvtClassDefs_Test.xml` to `${FERMI_INSTALL_REFDATADIR}/fitsGen`,
    which is what `getDataPath("fitsGen")` resolves to. The rest of
    `data/` is not installed: `FT1variables`, `irfTupleNames`, the cut
    files, `egret_gtis_tjd.dat`, `merit*.root`, `pointing_history.*`,
    `makeD2.py` and `jobOptions_rocking.txt`. Only the unbuilt programs
    use them.
- **`test_fitsGen`** (`src/test/test.cxx`):
  - It is installed to `bin/` and is **not registered with CTest**.
  - With `BUILD_WITHOUT_ROOT` it runs one thing: build
    `EventClassifier("Pass4_Classifier")` and classify four hand-made rows.
    `xml_classifier()` and `test_MeritFile2()` are compiled out.
  - `Pass4_Classifier.py` is not installed, and `$FERMI_INST_DIR/fitsGen/python`
    does not exist, so the import works only if the current directory is
    `src/fitsGen/python` (embed_python puts `.` first on `sys.path`).
    Anywhere else the import fails. The exception is **caught and printed**,
    and `main` still returns 0.
  - It has no assertions on the classifier output; it only prints the
    class numbers. In practice it is a smoke test, not a check.
  - It writes no files.
  - `Ft1File`, `Ft2File` and `FtFileBase` have **no test here**. They are
    exercised only through `test_observationSim` and `gtobssim`.
- **Legacy:** `SConscript`, `fitsGenLib.py` and `azure-pipelines.yml`
  belong to the old SCons/Azure build.

## Invariants and gotchas

**Time keywords**
- `TSTART`/`TSTOP` default to **−1**. If the caller never calls
  `setObsTimes`, the file is written with `TSTART = TSTOP = -1` and
  `DATE-OBS` one second before the mission start. `Ft1File::verifyObsTimes`
  would have inferred them from the `TIME` column, but its call in
  `close()` is commented out.
- `writeDateKeywords` ignores its `extension` argument. `TSTART`/`TSTOP`
  are written to the **primary HDU** as well as the extensions. The doc
  comment says it also writes `ONTIME`; it does not.
- `DATE-OBS`/`DATE-END` are `missionStart + MET/86400` as a calendar date,
  with no leap-second or TT−UTC correction, so they are approximate
  (inferred from the code). `DATE` is the current UTC time (`gmtime`).
- `setMissionStart` is **static** and process-wide (the unbuilt `egret2FT1`
  set it to 1991-04-15). Any later file in the same process uses it too.

**Templates**
- Only the four exact names `ft1.tpl`, `ft1_p7.tpl`, `ft2.tpl`, `lle.tpl`
  are looked up in `getDataPath("fitsGen")`. Any other string is used as a
  file path (relative to the current directory). An empty string means
  "append a new table to an existing file" rather than create one.
- With `FERMI_INST_DIR` unset the template path becomes
  `/refdata/fermi/fitsGen/ft1.tpl` and `createFile` fails.
- The templates say `TELESCOP = 'GLAST'`, not `'FERMI'`. Tools that switch
  on `TELESCOP` (for example `evtbin`'s `BinConfig`) accept both.
- `EVENT_CLASS` is `32X` in `ft1.tpl` and `I` in `ft1_p7.tpl`. The caller
  must set it with a `tip::BitStruct` for the first and an integer for the
  second (`observationSim` picks the template from `PASS_VER`).

**Event classifier (only reachable from `test_fitsGen`)**
- `operator()` returns `unsigned int`, but the Python convention is −1 for
  "no class", so an unclassified event comes back as `4294967295`.
- `python/EventClassifier.py` creates variables with `exec` inside a method
  and then reads them with `eval`. That relies on pre-3.13 `locals()`
  behaviour. Under Python 3.13 and later (PEP 667) the `eval` should raise
  `NameError` (from reading the code; not checked at runtime).
- `pythonPath()` points at `$FERMI_INST_DIR/fitsGen/python`, which is never
  installed. See `src/embed_python/doc/architecture/` for the `sys.path` and
  `Py_Finalize` behaviour of the `Module` it uses.

**Linking**
- `EventClassifier.h` puts `embed_python`, and so libpython, on the
  `PUBLIC` link interface. `observationSim` and `gtobssim` link libpython
  without using it.
- `EventClassifier`'s vtable layout depends on `BUILD_WITHOUT_ROOT`.
  Today every consumer sees the define through `tip`'s `PUBLIC` compile
  definition, so the layouts agree. A consumer that compiled
  `EventClassifier.h` without it would disagree with the library about the
  vtable (inferred).

**Dead code that ships or sits in the tree**
- `src/makeFT1`, `src/makeFT1_kluge`, `src/makeFT2`, `src/makeFT2a`,
  `src/convertFT1`, `src/egret2FT1`, `src/irfTuple` and `src/partition`
  have no CMake target, and the old SConscript did not build them either.
  Their par files (`makeFT1.par`, `makeFT1_kluge.par`, `makeFT2.par`) are
  still installed into `syspfiles`. `makeFT2`/`makeFT2a` call
  `astro::EarthCoordinate::geolat()`, which throws after the IGRF model
  ends (see `src/astro/doc/architecture/`). That doesn't matter while they
  are unbuilt.
- `MeritFile2.h` and `XmlEventClassifier.h` are installed but cannot be
  compiled without ROOT and `evtUtils`.
- `data/merit.root`, `merit_rocking.root` and `pointing_history.root` are
  committed binary test data for the unbuilt tools.

**Unmerged branches** (as of `last_reviewed_commit`)
- `origin/py-stable-abi` (2026-10) is a 4-line change to
  `EventClassifier.cxx` for the Python limited API.
- `origin/f26-cpp17-compliance` (2026-10) contains C++17 cleanups in
  `MeritFile.cxx` and the test.
- `origin/addition_creator_fits` (2025-08) changes `CREATOR` handling in
  the unbuilt `makeFT2`/`makeFT2a`.
- `origin/Issue17` (2022) is a one-line CMake change (optionally static
  libraries).

## Extension points

- **New FT1/FT2 column:** add it to the template in `data/` (and to both
  `ft1.tpl` and `ft1_p7.tpl` for FT1). Callers can also add a column at run
  time with `appendField`, as `observationSim` does for `MC_SRC_ID` and
  `MCENERGY`. Downstream readers (`dataSubselector`, `Likelihood`) only need
  the template change if they look for the column.
- **New file type:** add a template, add its name to the list in
  `FtFileBase::init` so it is found through `getDataPath`, add it to the
  `install(FILES ...)` list in `CMakeLists.txt`, and subclass `FtFileBase`
  if `close()` needs extra HDUs stamped.
- **Reviving a tool** (`makeFT2`, for example): add an `add_executable`
  linking `fitsGen st_app st_stream`, and expect to fix the IGRF expiry and
  install the `data/` files it reads.

After changing `FtFileBase` or the templates, run `test_observationSim`
and a short `gtobssim`, because nothing in this package tests the writers.

## Further reading

- `src/observationSim/doc/architecture/`: the only user.
- `src/tip/doc/architecture/`: the table/iterator model these classes wrap.
- `src/embed_python/doc/architecture/`: the embedded-interpreter details
  behind `EventClassifier`.
- `doc/FT1_Documentation.html`: historic FT1 column descriptions. The
  current definitions are in the FSSC "LAT Data Products" pages.
- `doc/release.notes`: history to 2015.
