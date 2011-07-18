#ifndef TrackingEfficiencyVsMomentumPlot_hh
#define TrackingEfficiencyVsMomentumPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TCanvas;

class TrackingEfficiencyVsMomentumPlot :
  public AnalysisPlot,
  public H2DPlot
{
  
public:
  enum Type {
    Positive,
    Negative,
    All
  };
  TrackingEfficiencyVsMomentumPlot(Type type);
  ~TrackingEfficiencyVsMomentumPlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  void finalize();
private:
  Type m_type;

};

#endif /* TrackingEfficiencyVsMomentumPlot_hh */
