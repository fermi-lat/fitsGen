/**
 * @file Util.cxx
 * @brief Utilities for fitsGen applications.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/src/Util.cxx,v 1.8 2006/02/03 06:03:42 jchiang Exp $
 */

#include <cstdlib>
#include <cstring>
#include <ctime>

#include <iostream>
#include <stdexcept>

#include "facilities/Util.h"

#include "st_facilities/Env.h"

#include "fitsGen/Util.h"

namespace fitsGen {

void Util::getFileNames(int iargc, char * argv[], std::string & rootFile, 
                        std::string & fitsFile) {
   if (iargc == 1) {
//       std::string fitsGenRoot = std::getenv("FITSGENROOT");
//       rootFile = fitsGenRoot + "/data/" + rootFile;
      rootFile = st_facilities::Env::getDataDir("fitsGen") + "/" + rootFile;
   } else if (iargc == 2) {
      std::string app_name = argv[0];
      if (!std::strcmp(argv[1], "-h")) {
         std::cout << "usage: " 
                   << facilities::Util::basename(argv[0]) << " "
                   << "<root input file> " 
                   << "<fits output file> " << std::endl;
         std::exit(0);
      } else {
         rootFile = std::string(argv[1]);
      }
   } else if (iargc >= 3) {
      rootFile = std::string(argv[1]);
      fitsFile = std::string(argv[2]);     
   }
}

} // namespace fitsGen

