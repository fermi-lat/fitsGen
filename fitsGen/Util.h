/**
 * @file Util.h
 * @brief Utilities for fitsGen apps.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/Util.h,v 1.2 2005/12/12 01:59:17 jchiang Exp $
 */

#ifndef fitsGen_Util_h
#define fitsGen_Util_h

#include <string>

#include "astro/JulianDate.h"

namespace tip {
   class Table;
}

namespace fitsGen {

/**
 * @class Util
 * @brief Static function utilities for fitsGen applications.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/Util.h,v 1.2 2005/12/12 01:59:17 jchiang Exp $
 */

class Util {

public:

   static void getFileNames(int iargc, char * argv[], std::string & rootFile,
                            std::string & fitsFile);

   static void writeDateKeywords(tip::Extension * table, double start_time,
                                 double stop_time, bool extension=true,
                                 const astro::JulianDate & mission_start
                                 =astro::JulianDate(2001, 1, 1, 0));

   static astro::JulianDate currentTime();

};

} // namespace fitsGen

#endif // fitsGen_Util_h
