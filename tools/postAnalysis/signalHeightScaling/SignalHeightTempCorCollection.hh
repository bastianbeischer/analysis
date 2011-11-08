#ifndef SignalHeightTempCorCollection_hh
#define SignalHeightTempCorCollection_hh

#include "PostAnalysisPlotCollection.hh"

class TFile;

class SignalHeightTempCorCollection : public PostAnalysisPlotCollection {
public:
  SignalHeightTempCorCollection(TFile*);
  ~SignalHeightTempCorCollection();
};

#endif
