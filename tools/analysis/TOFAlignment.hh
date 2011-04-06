#ifndef TOFAlignment_hh
#define TOFAlignment_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TOFAlignment : public AnalysisPlot, public H1DPlot {
public:
  TOFAlignment();
  virtual ~TOFAlignment();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
};

#endif
