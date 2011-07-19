#ifndef MultiLayerTrackingEfficiencyPlot_hh
#define MultiLayerTrackingEfficiencyPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class TH1D;
class TCanvas;

class MultiLayerTrackingEfficiencyPlot :
  public AnalysisPlot,
  public H2DProjectionPlot
{
  
public:
  enum Type {
    Positive,
    Negative,
    All
  };
  
  MultiLayerTrackingEfficiencyPlot(Type type);
  ~MultiLayerTrackingEfficiencyPlot();
  
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void finalize();

private:
  Type m_type;
  TH1D* m_normHisto;
  int m_nLayers;
  double* m_layerZ;
  
};

#endif /* MultiLayerTrackingEfficiencyPlot_hh */
