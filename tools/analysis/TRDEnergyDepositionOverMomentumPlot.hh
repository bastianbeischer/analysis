#ifndef TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH
#define TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TRDEnergyDepositionOverMomentumPlot: public AnalysisPlot, public H2DPlot
{

public:
  TRDEnergyDepositionOverMomentumPlot(AnalysisTopic);
  ~TRDEnergyDepositionOverMomentumPlot();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();

};

#endif // TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH
