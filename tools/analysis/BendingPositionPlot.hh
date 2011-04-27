#ifndef BendingPositionPlot_hh
#define BendingPositionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class BendingPositionPlot : public AnalysisPlot, public H1DPlot {
public:
  BendingPositionPlot();
  virtual ~BendingPositionPlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
};

#endif
