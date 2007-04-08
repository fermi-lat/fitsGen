/**
 * @file test.cxx
 * @brief Scaffolding for EventClassifier code.
 * @author J. Chiang
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/fitsGen/src/test/test.cxx,v 1.2 2006/12/11 19:09:52 jchiang Exp $
 */

#include <iostream>
#include <stdexcept>

#include "embed_python/Module.h"

#include "fitsGen/EventClassifier.h"

void run(fitsGen::EventClassifier & eventClass,
         double ctbgam, double ctbcore, int tkrLayer=5) {
   std::map<std::string, double> my_row;
   my_row["CTBSummedCTBGAM"] = ctbgam;
   my_row["CTBCORE"] = ctbcore;
   my_row["Tkr1FirstLayer"] = tkrLayer;
   my_row["CTBBestEnergyProb"] = 0.2;
   std::cout << eventClass(my_row) << std::endl;
}

int main() {
   try {
      fitsGen::EventClassifier foo("Pass4_Classifier");
      run(foo, 0.6, 0.8, 10);
      run(foo, 0.6, 0.7, 5);
      run(foo, 0.6, 0.3);
      run(foo, 0.3, 0.8);
   } catch (std::exception & eObj) {
      std::cout << eObj.what() << std::endl;
   }
}
