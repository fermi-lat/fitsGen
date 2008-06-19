# -*- python -*-
#
# $Id: SConscript,v 1.4 2008/03/19 21:05:21 glastrm Exp $
# Authors: James Chiang <jchiang@slac.stanford.edu>
# Version: fitsGen-04-00-00
Import('baseEnv')
Import('listFiles')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

if baseEnv['PLATFORM'] != "win32":
    progEnv.Append(CPPDEFINES = ['TRAP_FPE'])

libEnv.Tool('fitsGenLib', depsOnly = 1)
fitsGenLib = libEnv.StaticLibrary('fitsGen', listFiles(['src/*.cxx']))

progEnv.Tool('fitsGenLib')

makeFT1Bin = progEnv.Program('makeFT1', 'src/makeFT1/makeFT1.cxx')
makeFT2Bin = progEnv.Program('makeFT2', 'src/makeFT2/makeFT2.cxx')
makeFT2aBin = progEnv.Program('makeFT2a', 'src/makeFT2a/makeFT2a.cxx')
egret2FT1Bin = progEnv.Program('egret2FT1', listFiles(['src/egret2FT1/*.cxx']))
convertFT1Bin = progEnv.Program('convertFT1', 'src/convertFT1/convertFT1.cxx')
partitionBin = progEnv.Program('partition', 'src/partition/partition.cxx')
irfTupleBin = progEnv.Program('irfTuple', listFiles(['src/irfTuple/*.cxx']))
test_classifierBin = progEnv.Program('test_classifier', 
                                     listFiles(['src/test/*.cxx']))

progEnv.Tool('registerObjects', package = 'fitsGen', 
             libraries = [fitsGenLib], 
             binaries = [makeFT1Bin, makeFT2Bin, makeFT2aBin, egret2FT1Bin, 
                         convertFT1Bin, partitionBin, irfTupleBin,
                         test_classifierBin],
             includes = listFiles(['fitsGen/*.h']), 
             pfiles = listFiles(['pfiles/*.par']),
             data = listFiles(['data/*'], recursive = True))
