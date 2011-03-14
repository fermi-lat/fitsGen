# -*- python -*-
#
# $Id: SConscript,v 1.44 2011/03/11 21:05:21 heather Exp $
# Authors: James Chiang <jchiang@slac.stanford.edu>
# Version: fitsGen-06-02-03

import os
Import('baseEnv')
Import('listFiles')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

fitsGenLib = libEnv.StaticLibrary('fitsGen', listFiles(['src/*.cxx']))

progEnv.Tool('fitsGenLib')
if baseEnv['PLATFORM'] == "posix":
    progEnv.Append(CPPDEFINES = 'TRAP_FPE')
test_classifierBin = progEnv.Program('test_fitsGen', 
                                     listFiles(['src/test/*.cxx']))

dataFiles = [os.path.join("data", x) for x in ("xml_test_merit.root",
                                               "EvtClassDefs_Test.xml",
                                               "ft1.tpl", "ft2.tpl", "lle.tpl")]

progEnv.Tool('registerTargets', package = 'fitsGen', 
             staticLibraryCxts = [[fitsGenLib, libEnv]], 
             binaryCxts = [[test_classifierBin,progEnv]],
             includes = listFiles(['fitsGen/*.h']), 
             recursive = True,
             data = dataFiles)

