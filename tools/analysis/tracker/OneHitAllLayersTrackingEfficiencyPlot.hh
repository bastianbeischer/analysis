#ifndef OneHitAllLayersTrackingEfficiencyPlot_hh
#define OneHitAllLayersTrackingEfficiencyPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;
class TCanvas;

class OneHitAllLayersTrackingEfficiencyPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  enum Type {
    Positive,
    Negative,
    All
  };
  
  OneHitAllLayersTrackingEfficiencyPlot(Type type);
  ~OneHitAllLayersTrackingEfficiencyPlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  void update();
  void finalize();

private:
  Type m_type;
  TH1D* m_afterCutHisto;
  TH1D* m_normHisto;
  
};

#endif /* OneHitAllLayersTrackingEfficiencyPlot_hh */
