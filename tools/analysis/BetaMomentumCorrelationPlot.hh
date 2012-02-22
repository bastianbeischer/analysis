#ifndef BetaMomentumCorrelationPlot_hh
#define BetaMomentumCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;
class TF1;

class BetaMomentumCorrelationPlot : public AnalysisPlot, public H2DPlot {
public:
  BetaMomentumCorrelationPlot(bool hypothesisRigidity, bool hypothesisBeta);
  virtual ~BetaMomentumCorrelationPlot();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
private:
  TF1* newParticleFunction(Enums::Particle, double range);
  bool m_hypothesisRigidity;
  bool m_hypothesisBeta;
};

#endif
