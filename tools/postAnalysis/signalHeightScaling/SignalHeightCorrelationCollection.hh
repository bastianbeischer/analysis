#ifndef SignalHeightCorrelationCollection_hh
#define SignalHeightCorrelationCollection_hh

#include "PostAnalysisPlotCollection.hh"

class TFile;

class SignalHeightCorrelationCollection : public PostAnalysisPlotCollection {
public:
  enum Type {Time, Time2D, Temperature};
  SignalHeightCorrelationCollection(Type, TFile*);
  ~SignalHeightCorrelationCollection();
};

#endif
