/**
 * @file FtFileBase.h
 * @brief Declaration of FT1/2 file base class
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/FtFileBase.h,v 1.3 2006/01/10 18:41:30 jchiang Exp $
 */

#ifndef fitsGen_FtFileBase_h
#define fitsGen_FtFileBase_h

#include "tip/Table.h"

#include "astro/JulianDate.h"

namespace tip {
   class Header;
}

namespace fitsGen {

/**
 * @class FtFileBase
 * @brief Base class for FT1/2 files.
 * files.
 *
 * @author J. Chiang
 */

class FtFileBase {

public:

   FtFileBase(const std::string & outfile, long nrows=0);

   virtual ~FtFileBase();

   virtual void close();

   void next();

   void prev();

   tip::TableCell & operator[](const std::string & fieldname) {
      return (*m_it)[fieldname];
   }

   long nrows() const {
      return m_nrows;
   }

   void setNumRows(long nrows);

   const std::vector<std::string> & getFieldNames() const;

   void appendField(const std::string & colname, const std::string & format);

   tip::Table::Iterator begin();

   tip::Table::Iterator end();

   tip::Table::Iterator & itor();

   /// @return The EVENTS or SC_DATA extension FITS header.
   tip::Header & header();

   void setObsTimes(double start, double stop);

   static void setMissionStart(int year, int month, int day, int sec);

   static const astro::JulianDate & missionStart() {
      return s_missionStart;
   }

protected:

   std::string m_outfile;
   tip::Table * m_table;
   tip::Table::Iterator m_it;
   long m_nrows;

   double m_startTime;
   double m_stopTime;

   static astro::JulianDate s_missionStart;

   void init(const std::string & templateFile,
             const std::string & extname);

};

} // namespace fitsGen

#endif // fitsGen_FtFileBase_h
