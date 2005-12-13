/**
 * @file makeFT1.cxx
 * @brief Convert merit ntuple to FT1 format.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/src/makeFT1/makeFT1.cxx,v 1.8 2005/12/12 01:59:18 jchiang Exp $
 */

#include <iostream>
#include <stdexcept>
#include <string>

#include "dataSubselector/Gti.h"
#include "dataSubselector/Cuts.h"

#include "astro/SkyDir.h"

#include "st_facilities/FitsUtil.h"

#include "fitsGen/Ft1File.h"
#include "fitsGen/MeritFile.h"
#include "fitsGen/Util.h"

using namespace fitsGen;

int main(int iargc, char * argv[]) {
   std::string rootFile("merit.root");
   std::string fitsFile("myLatData.fits");
   
   Util::getFileNames(iargc, argv, rootFile, fitsFile);
   if (iargc == 1) {
      std::cout << "input merit file: " << rootFile << "\n"
                << "output FT1 file: " << fitsFile << std::endl;
   }

   dataSubselector::Cuts my_cuts;
   try {
      fitsGen::MeritFile merit(rootFile, "MeritTuple");
      fitsGen::Ft1File ft1(fitsFile, merit.nrows());
   
      int ncount(0);
      for ( ; merit.itor() != merit.end(); merit.next(), ft1.next()) {
         ft1["energy"].set(merit["FT1Energy"]);
         double ra = merit["FT1Ra"];
         double dec = merit["FT1Dec"];
         ft1["ra"].set(ra);
         ft1["dec"].set(dec);
//          ft1["l"].set(merit["FT1L"]);
//          ft1["b"].set(merit["FT1B"]);
         astro::SkyDir dir(ra, dec);
         ft1["l"].set(dir.l());
         ft1["b"].set(dir.b());
         ft1["theta"].set(merit["FT1Theta"]);
         ft1["phi"].set(merit["FT1Phi"]);
         ft1["zenith_angle"].set(merit["FT1ZenithTheta"]);
         ft1["earth_azimuth_angle"].set(merit["FT1EarthAzimuth"]);
         ft1["time"].set(merit["EvtElapsedTime"]);
         ft1["event_id"].set(static_cast<int>(merit["FT1EventId"]));
         if (17 - merit["Tkr1FirstLayer"] < 11.5) { // Front converting
            ft1["event_class"].set(0);
            ft1["conversion_type"].set(0);
         } else { // Back converting
            ft1["event_class"].set(1);
            ft1["conversion_type"].set(1);
         }
//         ft1["livetime"].set(merit["FT1Livetime"]);
         ncount++;
      }
      std::cout << "number of rows processed: " << ncount << std::endl;

      ft1.setNumRows(ncount);
      my_cuts.addGtiCut(merit.gti());
      my_cuts.writeDssKeywords(ft1.header());
   } catch (std::exception & eObj) {
      std::cout << eObj.what() << std::endl;
      return 1;
   }
   my_cuts.writeGtiExtension(fitsFile);
   st_facilities::FitsUtil::writeChecksums(fitsFile);
}

