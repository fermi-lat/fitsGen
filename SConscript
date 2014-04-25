# -*- python -*-
#
# $Id: SConscript,v 1.62 2014/04/14 23:34:28 jchiang Exp $
# Authors: James Chiang <jchiang@slac.stanford.edu>
# Version: fitsGen-07-00-01

import os
Import('baseEnv')
Import('listFiles')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('addLinkDeps', package = 'fitsGen', toBuild='static')
fitsGenLib = libEnv.StaticLibrary('fitsGen', listFiles(['src/*.cxx']))

progEnv.Tool('fitsGenLib')
if baseEnv['PLATFORM'] == "posix":
    progEnv.Append(CPPDEFINES = 'TRAP_FPE')
test_classifierBin = progEnv.Program('test_fitsGen', 
                                     listFiles(['src/test/*.cxx']))

dataFiles = [os.path.join("data", x) for x in ("xml_test_merit.root",
                                               "EvtClassDefs_Test.xml",
                                               "ft1.tpl", "ft1_p7.tpl", 
                                               "ft2.tpl", "lle.tpl")]

progEnv.Tool('registerTargets', package = 'fitsGen', 
             staticLibraryCxts = [[fitsGenLib, libEnv]], 
             binaryCxts = [[test_classifierBin,progEnv]],
             includes = listFiles(['fitsGen/*.h']), 
             recursive = True,
             data = dataFiles)

