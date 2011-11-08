#ifndef SignalHeightProjectionCollection_hh
#define SignalHeightProjectionCollection_hh

#include "PostAnalysisPlotCollection.hh"

class SignalHeightProjectionCollection : public PostAnalysisPlotCollection {
public:
  SignalHeightProjectionCollection(PostAnalysisCanvas*);
  ~SignalHeightProjectionCollection();
};

#endif
