#ifndef SignalHeightTimeCorCollection_hh
#define SignalHeightTimeCorCollection_hh

#include "PostAnalysisPlotCollection.hh"

class TFile;

class SignalHeightTimeCorCollection : public PostAnalysisPlotCollection {
public:
  SignalHeightTimeCorCollection(TFile*);
  ~SignalHeightTimeCorCollection();
};

#endif
