"""
@brief The trivial classifier that just copies the FT1EventClass value.
"""
#
# $Header: /nfs/slac/g/glast/ground/cvs/ScienceTools-scons/fitsGen/python/Pass6_kluge_Classifier.py,v 1.2 2008/07/13 14:30:21 jchiang Exp $
#
meritVariables = """
FT1EventClass
""".split()

eventClassifier = lambda row : row['FT1EventClass']
