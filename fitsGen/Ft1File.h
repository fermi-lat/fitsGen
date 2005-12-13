/**
 * @file Ft1File.h
 * @brief Declaration of FT1 file abstraction
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/Ft1File.h,v 1.5 2005/12/13 05:16:09 jchiang Exp $
 */

#ifndef fitsGen_Ft1File_h
#define fitsGen_Ft1File_h

#include "fitsGen/FtFileBase.h"

namespace fitsGen {

/**
 * @class Ft1File
 * @brief Abstraction/interface layer for using tip to write FT1
 * files.
 *
 * @author J. Chiang
 */

class Ft1File : public FtFileBase {

public:

   Ft1File(const std::string & outfile, long nrows=0,
           const std::string & templateFile="ft1.tpl");

   virtual void close();

private:

   void verifyObsTimes();

};

} // namespace fitsGen

#endif // fitsGen_Ft1File_h
