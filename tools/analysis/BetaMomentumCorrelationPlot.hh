#ifndef BetaMomentumCorrelationPlot_hh
#define BetaMomentumCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;
class TF1;

class BetaMomentumCorrelationPlot : public AnalysisPlot, public H2DPlot {
public:
  explicit BetaMomentumCorrelationPlot();
  virtual ~BetaMomentumCorrelationPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
};

#endif
