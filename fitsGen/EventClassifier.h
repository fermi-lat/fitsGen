/**
 * @file EventClassifier.h
 * @brief Set event classes for writing to FT1 files using
 * embed_python and a Python class to do the TCut parsing and event
 * partitioning and event class number assignment.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/EventClassifier.h,v 1.2 2006/12/11 03:55:52 jchiang Exp $
 */

#ifndef fitsGen_EventClassifier_h
#define fitsGen_EventClassifier_h

#include <string>
#include <vector>

#include "embed_python/Module.h"

namespace tip {
   class ConstTableRecord;
}

namespace fitsGen {

/**
 * @class EventClassifier
 * @brief Set event classes for writing to FT1 files using
 * embed_python and a Python class to do the TCut parsing and event
 * partitioning and event class number assignment.
 *
 * @author J. Chiang
 */

class EventClassifier {

public:

   EventClassifier(const std::string & classifierScript);

   ~EventClassifier() throw();

   long operator()(tip::ConstTableRecord & row);

   long operator()(const std::map<std::string, double> & row);

private:

   embed_python::Module * m_module;

   PyObject * m_classifier;

   /**
    * @class Nested class to encapsulate relevant merit variables for
    * access from event classifier script.
    */

   class MeritDict {

   public:

      MeritDict() : m_dict(0) {}

      MeritDict(embed_python::Module * module);

      ~MeritDict() throw();

      void setItem(const std::string & key,
                   double value);

      void setItems(tip::ConstTableRecord & row);

      double getItem(const std::string & key) const;

      std::vector<std::string> keys() const {
         return m_keys;
      }

      PyObject * pyDict() {
         return m_dict;
      }

   private:

      std::vector<std::string> m_keys;

      PyObject * m_dict;

   } * m_meritDict;

   std::string pythonPath() const;

};

} // namespace fitsGen

#endif // fitsGen_EventClassifier_h
