/**
 * @file MeritFile.h
 * @brief Declaration for MeritTuple abstraction.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/MeritFile.h,v 1.5 2006/02/06 00:36:59 jchiang Exp $
 */

#ifndef fitsGen_MeritFile_h
#define fitsGen_MeritFile_h

namespace dataSubselector {
   class Cuts;
   class Gti;
}

#include "tip/Table.h"

namespace fitsGen {

/**
 * @class MeritFile
 * @brief Abstraction/interface layer for using tip to read merit
 * files.
 *
 * @author J. Chiang
 */

class MeritFile {

public:

   MeritFile(const std::string & meritfile,
             const std::string & tree="MeritTuple",
             const std::string & filter="");

   ~MeritFile();

   void next();

   void prev();

   double operator[](const std::string & fieldname) const;

   tip::ConstTableRecord & row() const {
      return m_row;
   }

   long nrows() const {
      return m_nrows;
   }

   tip::Table::ConstIterator begin() const;

   tip::Table::ConstIterator end() const;

   tip::Table::ConstIterator & itor();

   /// @return A Gti object containing the GTIs for this merit file.
   /// This comprises just the beginning and end times for the data.
   const dataSubselector::Gti & gti() const;

   /// @return Event type of current row.
   short int eventType() const;

   /// @return Conversion type (e.g., front=0, back=1) of current row.
   short int conversionType() const;

private:

   const tip::Table * m_table;
   tip::Table::ConstIterator m_it;
   tip::ConstTableRecord & m_row;
   long m_nrows;

   bool m_haveTime;

   dataSubselector::Gti * m_gti;

   dataSubselector::Cuts * m_goodEvent1;
   dataSubselector::Cuts * m_goodEvent3;
};

} // namespace fitsGen

#endif // fitsGen_MeritFile_h
