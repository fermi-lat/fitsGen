/**
 * @file makeFT1.cxx
 * @brief Convert merit ntuple to FT1 format using Goodi.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/src/makeFT1.cxx,v 1.2 2003/10/15 15:27:56 jchiang Exp $
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

// Create the RootTuple objects, one each for the FT1 and Exposure
// branches in the merit file.
   RootTuple::RootTuple ft1Tuple(rootFile, "FT1");
   RootTuple::RootTuple exposureTuple(rootFile, "Exposure");

   std::vector<std::string> colNames;

   std::string query("");
   int nentries(0);

// Read in all of the columns for the FT1 and Exposure branches.
   colNames = ft1Tuple.branchNames();
   ft1Tuple.readTree(colNames, query, nentries);

   colNames = exposureTuple.branchNames();
   exposureTuple.readTree(colNames, query, nentries);

// Goodi setup.

// Create the IOService and Data factories.
   Goodi::DataIOServiceFactory iosvcCreator;
   Goodi::DataFactory dataCreator;

// Set the type of data to be generated and the mission.
   Goodi::DataType datatype = Goodi::Evt;
   Goodi::Mission  mission  = Goodi::Lat;

// Create the EventData object.
   Goodi::IEventData *data =
      dynamic_cast<Goodi::IEventData *>(dataCreator.create(datatype, mission));

   std::vector<double> vect_times = ft1Tuple("time");
   std::stable_sort(vect_times.begin(),vect_times.end());
   std::vector<std::pair<double,double> > gti;
   gti.push_back(std::make_pair(vect_times.front(),vect_times.back()));

   unsigned int nevts = vect_times.size();
   std::cout << "Number of events: " << nevts << std::endl;

// Fill the EventData object with data from the FT1 tree:
   
// ENERGY
   std::vector<double> energy = ft1Tuple("energy");
// Convert from MeV to eV, which is what Goodi wants.
   std::transform( energy.begin(), energy.end(), energy.begin(), 
                   std::bind2nd(std::multiplies<double>(), 1e6) );
   data->setEnergy(energy);

// Goodi wants angles in radians.
   std::vector<double> angles(nevts);
// RA
   std::transform( ft1Tuple("ra").begin(), 
                   ft1Tuple("ra").end(), 
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setRA(angles);
//   data->setRA(ft1Tuple("ra"));

// DEC
   std::transform( ft1Tuple("dec").begin(), 
                   ft1Tuple("dec").end(), 
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setDec(angles);
//   data->setDec(ft1Tuple("dec"));

// Colatitude and azimuthal angle in instrument coordinates.
// THETA
   std::transform( ft1Tuple("theta").begin(), 
                   ft1Tuple("theta").end(), 
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setTheta(angles);
//   data->setTheta( ft1Tuple("theta") );

// PHI
   std::transform( ft1Tuple("phi").begin(), 
                   ft1Tuple("phi").end(), 
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setPhi(angles);
//   data->setPhi(ft1Tuple("phi"));

// ZENITH_ANGLE
   std::transform( ft1Tuple("zenith_angle").begin(), 
                   ft1Tuple("zenith_angle").end(),
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setZenithAngle(angles);
//   data->setZenithAngle(ft1Tuple("zenith_angle"));

// EARTH_AZIMUTH
   std::transform( ft1Tuple("earth_azimuth").begin(), 
                   ft1Tuple("earth_azimuth").end(),
                   angles.begin(), 
                   std::bind2nd(std::multiplies<double>(), M_PI/180.) );
   data->setAzimuth(angles);
//   data->setAzimuth(ft1Tuple("earth_azimuth"));

// TIME
   data->setTime(ft1Tuple("time"));

// EVENT_ID
// Using std::copy complains about type conversion; passing
// static_cast<long>() as the unary function to transform does not to
// compile under gcc 2.95.3.  This kludge seems to work without
// complaint.
   std::vector<long> eventId(nevts);
   std::transform( ft1Tuple("event_id").begin(), 
                   ft1Tuple("event_id").end(),
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
      = ft1Tuple("convPointX").begin();
   std::vector<double>::const_iterator pConvPtY 
      = ft1Tuple("convPointY").begin();
   std::vector<double>::const_iterator pConvPtZ 
      = ft1Tuple("convPointZ").begin();
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
   std::transform( ft1Tuple("convLayer").begin(), 
                   ft1Tuple("convLayer").end(),
                   convLayer.begin(), 
                   std::bind2nd(std::multiplies<int>(), 1) );
   data->setConvLayer( convLayer );

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
//    data->setConvLayer();
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

   data->write(ioService, latFile);
   delete ioService;
}
