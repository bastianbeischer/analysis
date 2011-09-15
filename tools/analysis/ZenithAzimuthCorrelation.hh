#ifndef ZenithAzimuthCorrelation_hh
#define ZenithAzimuthCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class ZenithAzimuthCorrelation : public AnalysisPlot, public H2DPlot {
public:
  ZenithAzimuthCorrelation();
  ~ZenithAzimuthCorrelation();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
};

#endif /* ZenithAzimuthCorrelation_hh */
