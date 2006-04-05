/**
 * @file Util.h
 * @brief Utilities for fitsGen apps.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/Util.h,v 1.3 2005/12/15 04:38:00 jchiang Exp $
 */

#ifndef fitsGen_Util_h
#define fitsGen_Util_h

#include <string>

namespace fitsGen {

/**
 * @class Util
 * @brief Static function utilities for fitsGen applications.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/Util.h,v 1.3 2005/12/15 04:38:00 jchiang Exp $
 */

class Util {

public:

   static void getFileNames(int iargc, char * argv[], std::string & rootFile,
                            std::string & fitsFile);

};

} // namespace fitsGen

#endif // fitsGen_Util_h
