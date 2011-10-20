#ifndef TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH
#define TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TRDEnergyDepositionOverMomentumPlot: public AnalysisPlot, public H2DPlot
{

public:
  TRDEnergyDepositionOverMomentumPlot(Enums::AnalysisTopic);
  ~TRDEnergyDepositionOverMomentumPlot();

  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();

};

#endif // TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH
