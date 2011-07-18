#ifndef SingleLayerTrackingEfficiencyPlot_hh
#define SingleLayerTrackingEfficiencyPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TCanvas;

class SingleLayerTrackingEfficiencyPlot :
  public AnalysisPlot,
  public H2DPlot
{
  
public:
  enum Type {
    Positive,
    Negative,
    All
  };
  SingleLayerTrackingEfficiencyPlot(Type type);
  ~SingleLayerTrackingEfficiencyPlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  void finalize();

private:
  Type m_type;
  TH2D* m_normHisto;
  int m_nLayers;
  double* m_layerZ;
  
};

#endif /* SingleLayerTrackingEfficiencyPlot_hh */
