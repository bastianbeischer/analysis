#ifndef FluxCollection_hh
#define FluxCollection_hh

#include "PlotCollection.hh"

#include <QDateTime>

class TH1D;

class FluxCollection : public PlotCollection {
public:
  FluxCollection(const QDateTime& first, const QDateTime& last);
  ~FluxCollection();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  TH1D* m_particleHistogram;
  TH1D* m_particleHistogramAlbedo;
};

#endif /* FluxCollection_hh */
