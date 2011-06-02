/**
 * @file MeritFile2.cxx
 * @brief Interface to merit files that uses ROOT directly.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/ScienceTools-scons/fitsGen/src/MeritFile2.cxx,v 1.4 2011/02/16 06:29:25 jchiang Exp $
 */

#include <stdexcept>

#include "TChain.h"
#include "TError.h"
#include "TEventList.h"
#include "TFile.h"
#include "TLeaf.h"
#include "TTree.h"

#include "fitsGen/MeritFile2.h"

namespace fitsGen {

MeritFile2::MeritFile2(const std::string & meritfile,
                       const std::string & tree,
                       const std::string & filter) :
   m_file(0), m_tree(0), m_index(0) {
/// Vain effort to suppress uninformative error messages from ROOT.
/// This is borrowed from tip::RootTable.cxx.
   long root_err_level = gErrorIgnoreLevel;
   m_file = TFile::Open(meritfile.c_str());
   gErrorIgnoreLevel = root_err_level;

   if (m_file == 0) {
      throw std::runtime_error("Failed to load " + meritfile);
   }

   m_tree = dynamic_cast<TTree *>(m_file->Get(tree.c_str()));
   if (m_tree == 0) {
      throw std::runtime_error("Failed to load tree '" 
                               + tree + "' from file " + meritfile);
   }
   
// Use TTree::Draw function to apply the filter string and get a
// TEventList.
   Long64_t first(0);
   Long64_t nmax(m_tree->GetEntries());
   m_tree->Draw(">>merit_event_list", filter.c_str(), "", nmax, first);
   m_eventList = (TEventList *)gDirectory->Get("merit_event_list");

   m_nrows = m_eventList->GetN();

   m_tstart = operator[]("EvtElapsedTime");
   m_tree->GetEvent(m_eventList->GetEntry(m_nrows-1));
   m_tstop = operator[]("EvtElapsedTime");
   rewind();
}

MeritFile2::MeritFile2(const std::vector<std::string> & meritFiles,
                       const std::string & tree,
                       const std::string & filter) :
   m_file(0), m_tree(new TChain(tree.c_str())), m_index(0) {
/// Vain effort to suppress uninformative error messages from ROOT.
/// This is borrowed from tip::RootTable.cxx.
   long root_err_level = gErrorIgnoreLevel;
   gErrorIgnoreLevel = root_err_level;

   for (size_t i(0); i < meritFiles.size(); i++) {
      dynamic_cast<TChain *>(m_tree)->Add(meritFiles[i].c_str());
   }
   m_tree->SetBranchStatus("*", 1);
   
// Use TTree::Draw function to apply the filter string and get a
// TEventList.
   m_tree->Draw(">>merit_event_list", filter.c_str(), "");
   m_eventList = (TEventList *)gDirectory->Get("merit_event_list");

   m_nrows = m_eventList->GetN();

   m_tstart = operator[]("EvtElapsedTime");
   m_tree->GetEvent(m_eventList->GetEntry(m_nrows-1));
   m_tstop = operator[]("EvtElapsedTime");
   rewind();
}

MeritFile2::~MeritFile2() {
   BranchMap_t::iterator it = m_branches.begin();
   for ( ; it != m_branches.end(); ++it) {
      delete_branch_pointer(it->second);
   }
   delete m_file;
   if (dynamic_cast<TChain *>(m_tree)) {
      delete m_tree;
   }
}

Long64_t MeritFile2::next() {
   if (m_index < m_nrows) {
      m_index++;
      setEntry();
   }
   return m_index;
}

Long64_t MeritFile2::prev() {
   if (m_index > 0) {
      m_index--;
      setEntry();
   }
   return m_index;
}

Long64_t MeritFile2::rewind() {
   m_index = 0;
   setEntry();
   return m_index;
}

void MeritFile2::setEntry() {
   m_tree->GetEvent(m_eventList->GetEntry(m_index));
}

double MeritFile2::operator[](const std::string & fieldname) {
   BranchMap_t::iterator it = m_branches.find(fieldname);
   if (it == m_branches.end()) {
      BranchData_t branch_data(get_branch_pointer(fieldname));
      m_tree->SetBranchAddress(fieldname.c_str(), branch_data.first);
      m_branches[fieldname] = branch_data;
      setEntry();
      return recast_as_double(branch_data);
   }
   return recast_as_double(it->second);
}

short int MeritFile2::conversionType() const {
   double layer(const_cast<MeritFile2 *>(this)->operator[]("Tkr1FirstLayer"));
   if (17 - layer < 11.5) {
      return 0;
   }
   return 1;
}

//BranchData_t MeritFile2::
std::pair<void *, std::string> MeritFile2::
get_branch_pointer(const std::string & fieldname) const {
   TLeaf * leaf = m_tree->GetLeaf(fieldname.c_str());
   if (!leaf) {
      throw std::runtime_error("leaf " + fieldname + " not found.");
   }
   void * pointer(0);
   std::string type(leaf->GetTypeName());
   if (type == "Double_t") {
      pointer = new Double_t;
   } else if (type == "Float_t") {
      pointer = new Float_t;
   } else if (type == "Int_t") {
      pointer = new Int_t;
   } else if (type == "UInt_t") {
      pointer = new UInt_t;
   } else if (type == "Long_t") {
      pointer = new Long_t;
   } else if (type == "ULong_t") {
      pointer = new ULong_t;
   }
   return std::make_pair(pointer, type);
}

double MeritFile2::recast_as_double(const BranchData_t & branch_data) const {
   if (branch_data.second == "Double_t") {
      return static_cast<double>(*reinterpret_cast<Double_t *>
                                 (branch_data.first));
   } else if (branch_data.second == "Float_t") {
      return static_cast<double>(*reinterpret_cast<Float_t *>
                                 (branch_data.first));
   } else if (branch_data.second == "Int_t") {
      return static_cast<double>(*reinterpret_cast<Int_t *>
                                 (branch_data.first));
   } else if (branch_data.second == "UInt_t") {
      return static_cast<double>(*reinterpret_cast<UInt_t *>
                                 (branch_data.first));
   } else if (branch_data.second == "Long_t") {
      return static_cast<double>(*reinterpret_cast<Long_t *>
                                 (branch_data.first));
   } else if (branch_data.second == "ULong_t") {
      return static_cast<double>(*reinterpret_cast<ULong_t *>
                                 (branch_data.first));
   }
}

void MeritFile2::delete_branch_pointer(const BranchData_t & branch_data) const {
   if (branch_data.second == "Double_t") {
      delete reinterpret_cast<Double_t *>(branch_data.first);
   } else if (branch_data.second == "Float_t") {
      delete reinterpret_cast<Float_t *>(branch_data.first);
   } else if (branch_data.second == "Int_t") {
      delete reinterpret_cast<Int_t *>(branch_data.first);
   } else if (branch_data.second == "UInt_t") {
      delete reinterpret_cast<UInt_t *>(branch_data.first);
   } else if (branch_data.second == "Long_t") {
      delete reinterpret_cast<Long_t *>(branch_data.first);
   } else if (branch_data.second == "ULong_t") {
      delete reinterpret_cast<ULong_t *>(branch_data.first);
   }
}


} //namespace fitsGen
