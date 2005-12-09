/**
 * @file EgretSmdb.cxx
 * @brief Implementation for EGRET summary database file interface class.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/src/egret2FT1/EgretSmdb.cxx,v 1.1 2005/12/08 17:57:12 jchiang Exp $
 */

#include <cmath>

#include <algorithm>

#include "tip/IFileSvc.h"

#include "EgretSmdb.h"

double EgretSmdb::s_mjdref(48368.);

EgretSmdb::EgretSmdb(const std::string & smdbfile) :
   m_table(tip::IFileSvc::instance().readTable(smdbfile, "EGRET_SMDB")),
   m_it(m_table->begin()),
   m_row(*m_it),
   m_nrows(m_table->getNumRecords()) {
}

EgretSmdb::~EgretSmdb() {
   delete m_table;
}

void EgretSmdb::next() {
   ++m_it;
}

double EgretSmdb::operator[](const std::string & fieldname) const {
   return m_row[fieldname].get();
}

tip::Table::ConstIterator EgretSmdb::begin() const {
   return m_table->begin();
}

tip::Table::ConstIterator EgretSmdb::end() const {
   return m_table->end();
}

tip::Table::ConstIterator EgretSmdb::itor() const {
   return m_it;
}

double EgretSmdb::arrivalTime() const {
   double tjd = arrivalTjd();
   double met = (tjd + 4e4 - s_mjdref)*8.64e4;
   return met;
}

double EgretSmdb::arrivalTjd() const {
   double tjd(m_row["trunc_julian_day"].get());
   double msec(m_row["milliseconds"].get());
   double musec(m_row["microseconds"].get());

   return tjd + (msec + musec/1e3)/8.64e7;
}

bool EgretSmdb::tascIn() const {
   char tasc1, tasc2;
   m_row["TASC_#1_FLAGS"].get(tasc1);
   m_row["TASC_#2_FLAGS"].get(tasc2);
   int val = static_cast<int>(tasc1 & 32) + static_cast<int>(tasc2 & 32);
   if (!val) {
      return true;
   }
   return false;
}

bool EgretSmdb::zenAngleCut(double nsigma) const {
   static const double sig0(5.85);
   static const double indx(-0.534);
   double energy(m_row["gamma_ray_energy"].get());
   double maxZenAngle(110. - nsigma*sig0*std::pow(energy/100., indx));
   maxZenAngle = std::min(maxZenAngle, 105.);
   return m_row["zenith_direction"].get()*180./M_PI <= maxZenAngle;
}

int EgretSmdb::eventClass() const {
   char evt_class;
   m_row["ENERGY"].get(evt_class);
   return static_cast<int>(evt_class & 3);
}
