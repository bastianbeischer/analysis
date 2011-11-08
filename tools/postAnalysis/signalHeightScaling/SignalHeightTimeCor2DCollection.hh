#ifndef SignalHeightTimeCor2DCollection_hh
#define SignalHeightTimeCor2DCollection_hh

#include "PostAnalysisPlotCollection.hh"

class TFile;

class SignalHeightTimeCor2DCollection : public PostAnalysisPlotCollection {
public:
  SignalHeightTimeCor2DCollection(TFile*);
  ~SignalHeightTimeCor2DCollection();
};

#endif
