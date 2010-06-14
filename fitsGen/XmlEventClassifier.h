/**
 * @file XmlEventClassifier.h
 * @brief Wrap evtUtils code to read in xml event class definitions and
 * apply them to a merit file.
 *
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/fitsGen/XmlEventClassifier.h,v 1.2 2010/06/12 15:16:34 jchiang Exp $
 */

#ifndef fitsGen_XmlEventClassifier_h
#define fitsGen_XmlEventClassifier_h

#include <map>
#include <string>
#include <utility>

class TFile;

namespace evtUtils {
   class EventClass;
}

namespace fitsGen {

/**
 * @class XmlEventClassifier
 *
 */

class XmlEventClassifier {

public:

   XmlEventClassifier(const std::string & xmlFile,
                      const std::string & meritFile);

   ~XmlEventClassifier() throw();

   unsigned int operator()(unsigned int run,
                           unsigned int eventId) const;

   bool is_class_member(unsigned int run, 
                        unsigned int eventId, 
                        unsigned int evtclass) const;

private:

   evtUtils::EventClass * m_evtClass;
   TFile * m_meritFile;

   typedef std::map<std::pair<unsigned int, unsigned int>,
                    unsigned int> EventClassMap_t;
   
   EventClassMap_t m_bitMaps;
};

} // namespace fitsGen

#endif // fitsGen_XmlEventClassifier_h
