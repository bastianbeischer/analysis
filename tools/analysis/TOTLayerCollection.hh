#ifndef TOTLayerCollection_hh
#define TOTLayerCollection_hh

#include "PlotCollection.hh"

class TOTLayerCollection : public PlotCollection {
public:
  enum Type {Projection, TrdSignalCorrelation, TrackerSigalCorrelation, MomentumCorrelation, BetaCorrelation};

  TOTLayerCollection(Type);
  ~TOTLayerCollection();
};

#endif /* TOTLayerCollection_hh */
