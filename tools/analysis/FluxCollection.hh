#ifndef FluxCollection_hh
#define FluxCollection_hh

#include "PlotCollection.hh"

#include <QDateTime>

class TH1D;

class FluxCollection : public PlotCollection {
public:
  FluxCollection(int numberOfThreads);
  ~FluxCollection();
  void processEvent(const AnalyzedEvent*);
private:
  TH1D* m_particleHistogram;
  TH1D* m_particleHistogramAlbedo;
};

#endif /* FluxCollection_hh */
