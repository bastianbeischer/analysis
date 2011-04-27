#ifndef BendingAnglePlot_hh
#define BendingAnglePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class BendingAnglePlot : public AnalysisPlot, public H1DPlot {
public:
  BendingAnglePlot();
  virtual ~BendingAnglePlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void finalize();
private:
};

#endif
