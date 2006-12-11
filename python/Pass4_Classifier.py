"""
@brief Pass 4 event classes based on Toby's straw man proposal.

@author J. Chiang <jchiang@slac.stanford.edu>
"""
#
# $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/python/EventClassifier.py,v 1.2 2006/12/11 05:53:58 jchiang Exp $
#
from EventClassifier import EventClassifier

meritVariables = """
EvtRun    EvtEnergyCorr 
McEnergy  McXDir  McYDir  McZDir   
McXDirErr   McYDirErr  McZDirErr   
McTkr1DirErr  McDirErr  
GltWord   FilterStatus_HI 
Tkr1FirstLayer  
CTBCORE  CTBSummedCTBGAM  CTBBestEnergyProb
""".split()

#
# Example event class cuts.
#
eventClassCuts = ['CTBSummedCTBGAM>=0.5 && CTBCORE>=0.8',
                  'CTBSummedCTBGAM>=0.5 && CTBCORE>=0.5 && CTBCORE<0.8',
                  'CTBSummedCTBGAM>=0.5 && CTBCORE<0.5',
                  'CTBSummedCTBGAM>=0.1 && CTBSummedCTBGAM<0.5',
                  'CTBSummedCTBGAM<0.1']

eventClassifier = EventClassifier(eventClassCuts)
