/**
 * @file makeFT1.cxx
 * @brief Convert merit ntuple to FT1 format using Goodi.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/src/makeFT1.cxx,v 1.5 2003/11/15 00:47:24 jchiang Exp $
 */

#include <cmath>

#include <vector>
#include <valarray>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>

#include "Goodi/GoodiConstants.h"
#include "Goodi/DataIOServiceFactory.h"
#include "Goodi/DataFactory.h"
#include "Goodi/IDataIOService.h"
#include "Goodi/IData.h"
#include "Goodi/IEventData.h"

#include "Goodi/../src/LatEventsData.h"    // meh.

#include "rootTuple/RootTuple.h"

int main(int iargc, char * argv[]) {

   std::string rootFile;
   if (iargc == 1) {
      std::string fitsGenRoot = ::getenv("FITSGENROOT");
      rootFile = fitsGenRoot + "/data/merit.root";
   } else if (iargc == 2) {
      if (argv[1] == "-h") {
         std::cout << "usage: " 
                   << argv[0] << ": "
                   << "root_file" << std::endl;
         return 0;
      } else {
         rootFile = std::string(argv[1]);
      }
   }

// Create the RootTuple object, for the Exposure
// branches in the merit file.
   RootTuple::RootTuple exposureTuple(rootFile, "Exposure");

// Need the MeritTuple tree for the IM variables
   RootTuple::RootTuple meritTuple(rootFile, "MeritTuple");

   std::vector<std::string> colNames;

   std::string query("");
   int nentries(0);

// Read in the IM columns, CalEnergySum, CalTotRLn, GltLayer, FilterStatus_HI
   colNames.push_back("IMgoodCalProb");
   colNames.push_back("IMvertexProb");
   colNames.push_back("IMcoreProb");
   colNames.push_back("IMpsfErrPred");
   colNames.push_back("IMgammaProb");
   colNames.push_back("CalEnergySum");
   colNames.push_back("CalTotRLn");
   colNames.push_back("GltLayer");
   colNames.push_back("GltFilterStatusHI");
   meritTuple.readTree(colNames, query, nentries);

// Read in all of the columns for the FT1 and Exposure branches.
   colNames = meritTuple.branchNames();
   meritTuple.readTree(colNames, query, nentries);

   colNames = exposureTuple.branchNames();
   exposureTuple.readTree(colNames, query, nentries);

// Goodi setup.

// Create the IOService and Data factories.
   Goodi::DataIOServiceFactory iosvcCreator;
   Goodi::DataFactory dataCreator;

// Set the type of data to be generated and the mission.
   Goodi::DataType datatype = Goodi::Evt;
   Goodi::Mission  mission  = Goodi::Lat;

// Create the EventData object and explicitly down-cast to
// LatEventsData since the IEventData interface does not have all of
// the methods required to access LAT data.
   Goodi::LatEventsData *data =
      dynamic_cast<Goodi::LatEventsData *>
      (dataCreator.create(datatype, mission));

   std::vector<double> vect_times = meritTuple("EvtTime");
   std::stable_sort(vect_times.begin(),vect_times.end());
   std::vector<std::pair<double,double> > gti;
   gti.push_back(std::make_pair(vect_times.front(),vect_times.back()));

   unsigned int nevts = vect_times.size();
   std::cout << "Number of events: " << nevts << std::endl;

// Fill the EventData object with data from the FT1 tree:
   
// ENERGY
   std::vector<double> energy = meritTuple("FT1Energy");
// Convert from MeV to eV, which is what Goodi wants.
   std::transform( energy.begin(), energy.end(), energy.begin(), 
                   std::bind2nd(std::multiplies<double>(), 1e6) );
   data->setEnergy(energy);

// Goodi wants angles in radians.
   std::vector<double> angles(nevts);
// RA
   std::transform( meritTuple("FT1Ra").begin(), 
                   meritTuple("FT1Ra").end(), 
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setRA(angles);
//   data->setRA(meritTuple("ra"));

// DEC
   std::transform( meritTuple("FT1Dec").begin(), 
                   meritTuple("FT1Dec").end(), 
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setDec(angles);
//   data->setDec(meritTuple("dec"));

// Colatitude and azimuthal angle in instrument coordinates.
// THETA
   std::transform( meritTuple("FT1Theta").begin(), 
                   meritTuple("FT1Theta").end(), 
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setTheta(angles);
//   data->setTheta( meritTuple("theta") );

// PHI
   std::transform( meritTuple("FT1Phi").begin(), 
                   meritTuple("FT1Phi").end(), 
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setPhi(angles);
//   data->setPhi(meritTuple("phi"));

// ZENITH_ANGLE
   std::transform( meritTuple("FT1ZenithTheta").begin(), 
                   meritTuple("FT1ZenithTheta").end(),
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setZenithAngle(angles);
//   data->setZenithAngle(meritTuple("zenith_angle"));

// EARTH_AZIMUTH
   std::transform( meritTuple("FT1EarthAzimuth").begin(), 
                   meritTuple("FT1EarthAzimuth").end(),
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setAzimuth(angles);
//   data->setAzimuth(meritTuple("earth_azimuth"));

// TIME
   data->setTime(meritTuple("EvtTime"));

// EVENT_ID
// Using std::copy complains about type conversion; passing
// static_cast<long>() as the unary function to transform does not to
// compile under gcc 2.95.3.  This kludge seems to work without
// complaint.
   std::vector<long> eventId(nevts);
   std::transform( meritTuple("FT1EventId").begin(), 
                   meritTuple("FT1EventId").end(),
                   eventId.begin(), 
                   std::bind2nd(std::multiplies<long>(), 1) );
   data->setEventID(eventId);

// Set the sizes of the valarray data for the multiword columns,
    std::vector< std::valarray<float>  >      convPoint(nevts);
// GEO_OFFSET, BARY_OFFSET, etc.. and fill where possible.
//    std::vector< std::valarray<double> >      geoOffset(nevts);
//    std::vector< std::valarray<double> >     baryOffset(nevts);
//    std::vector< std::valarray<long>   >    acdTilesHit(nevts);
//    std::vector< std::valarray<int>    >   calibVersion(nevts);
    for (unsigned int i = 0; i < nevts; i++) {
       convPoint[i].resize(3);
//       geoOffset[i].resize(3);
//       baryOffset[i].resize(3);
//       acdTilesHit[i].resize(3);
//       calibVersion[i].resize(3);
    }
   
// CONVERSION_POINT
   std::vector<double>::const_iterator pConvPtX 
      = meritTuple("FT1ConvPointX").begin();
   std::vector<double>::const_iterator pConvPtY 
      = meritTuple("FT1ConvPointY").begin();
   std::vector<double>::const_iterator pConvPtZ 
      = meritTuple("FT1ConvPointZ").begin();
   std::vector< std::valarray<float> >::iterator pConvPoint 
      = convPoint.begin();
   while (pConvPoint != convPoint.end()) {
       (*pConvPoint)[0] = static_cast<float>(*pConvPtX);
       (*pConvPoint)[1] = static_cast<float>(*pConvPtY);
       (*pConvPoint)[2] = static_cast<float>(*pConvPtZ);

       pConvPoint++;
       pConvPtX++;
       pConvPtY++;
       pConvPtZ++;
   }
   data->setConvPoint(convPoint);

// CONVERSION_LAYER
   std::vector<int> convLayer(nevts);
   std::transform( meritTuple("FT1ConvLayer").begin(), 
                   meritTuple("FT1ConvLayer").end(),
                   convLayer.begin(), 
                   std::bind2nd(std::multiplies<int>(), 1) );
   data->setConvLayer( convLayer );

// IM variables
   assert(meritTuple("IMgoodCalProb").size() == nevts);

   std::vector<float> floatVector(nevts);
   std::copy( meritTuple("IMgoodCalProb").begin(),
              meritTuple("IMgoodCalProb").end(),
              floatVector.begin() );
   data->setGoodCalProb( floatVector );

   std::copy( meritTuple("IMvertexProb").begin(),
              meritTuple("IMvertexProb").end(),
              floatVector.begin() );
   data->setVertexProb( floatVector );

   std::copy( meritTuple("IMcoreProb").begin(),
              meritTuple("IMcoreProb").end(),
              floatVector.begin() );
   data->setCoreProb( floatVector );

   std::copy( meritTuple("IMpsfErrPred").begin(),
              meritTuple("IMpsfErrPred").end(),
              floatVector.begin() );
   data->setPsferrpred( floatVector );

   std::copy( meritTuple("IMgammaProb").begin(),
              meritTuple("IMgammaProb").end(),
              floatVector.begin() );
   data->setGammaProb( floatVector );

// // GltLayer (no doubt the same as ConvLayer)
//    std::vector<short> gltLayer(nevts);
//    std::transform( meritTuple("GltLayer").begin(), 
//                    meritTuple("GltLayer").end(),
//                    gltLayer.begin(), 
//                    std::bind2nd(std::multiplies<short>(), 1) );
//     data->setGltLayer( gltLayer );

// CalEnergySum and CalTotRLn
   std::copy( meritTuple("CalEnergySum").begin(),
              meritTuple("CalEnergySum").end(),
              floatVector.begin() );
   data->setCalEnergySum( floatVector );

   std::copy( meritTuple("CalTotRLn").begin(),
              meritTuple("CalTotRLn").end(),
              floatVector.begin() );
   data->setCalToTrln( floatVector );

// Set methods in IEventData that have no corresponding branches in
// the FT1 tree:
//    data->setWeight();
//    data->setMJD();
//    data->setTimeOfDay();
//    data->setEnergyUncert();
//    data->setPulsePhase();
//    data->setGeoTime();
//    data->setBaryTime();
//    data->setQuality();
//    data->setQualParams();
//    data->setReconVersion();
//    data->setMultiplicity();
//    data->setSubsysFlag();
//    data->setStartTime();
//    data->setStopTime();
   
// GTI infos: START and STOP times
   data->setGTI(gti);

// These also are not implemented in the FT1 tree, but Goodi seems to
// want these columns set.
//   data->setGeoOffset(geoOffset);
//   data->setBaryOffset(baryOffset);
//   data->setAcdTilesHit(acdTilesHit);
//   data->setCalibVersion(calibVersion);

// Write the data to a FITS file.
   std::string latFile("!myLatData.fits");
   Goodi::IDataIOService *ioService = iosvcCreator.create();

// Need to "up"-cast to have access to the write(...) method we need.
   Goodi::IEventData *idata = data;
   idata->write(ioService, latFile);

   delete ioService;
}




