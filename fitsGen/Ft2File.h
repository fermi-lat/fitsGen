/**
 * @file Ft2File.h
 * @brief Declaration of FT1 file abstraction
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/Ft2File.h,v 1.3 2005/12/12 01:59:17 jchiang Exp $
 */

#ifndef fitsGen_Ft2File_h
#define fitsGen_Ft2File_h

#include "fitsGen/FtFileBase.h"

namespace fitsGen {

/**
 * @class Ft2File
 * @brief Abstraction/interface layer for using tip to write FT2
 * files.
 *
 * @author J. Chiang
 */

class Ft2File : public FtFileBase {

public:

   Ft2File(const std::string & outfile, long nrows=0) 
      : FtFileBase(outfile, nrows) {
      init("ft2.tpl", "Ext1");
   }

};

} // namespace fitsGen

#endif // fitsGen_Ft2File_h
