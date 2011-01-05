#ifndef TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH
#define TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH

#include "AnalysisPlot.hh"
#include "GraphPlot.hh"

class TRDEnergyDepositionOverMomentumPlot: public AnalysisPlot, public GraphPlot
{

public:
  TRDEnergyDepositionOverMomentumPlot(AnalysisPlot::Topic);
  ~TRDEnergyDepositionOverMomentumPlot();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();

};

#endif // TRDENERGYDEPOSITIONOVERMOMENTUMPLOT_HH
