#ifndef BendingAnglePlot_hh
#define BendingAnglePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class BendingAnglePlot : public AnalysisPlot, public H1DPlot {
public:
  BendingAnglePlot();
  virtual ~BendingAnglePlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
private:
};

#endif
