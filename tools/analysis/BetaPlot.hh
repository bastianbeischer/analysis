#ifndef BetaPlot_hh
#define BetaPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class BetaPlot : public AnalysisPlot, public H1DPlot {
public:
  BetaPlot();
  virtual ~BetaPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void update();
};

#endif
