#ifndef SingleLayerTrackingEfficiencyPlot_hh
#define SingleLayerTrackingEfficiencyPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "Enums.hh"

class TCanvas;

class SingleLayerTrackingEfficiencyPlot : public AnalysisPlot, public H2DPlot
{
public:
  SingleLayerTrackingEfficiencyPlot(Enums::ChargeSigns);
  ~SingleLayerTrackingEfficiencyPlot();
  void processEvent(const AnalyzedEvent*);
  void finalize();

private:
  Enums::ChargeSigns m_type;
  TH2D* m_normHisto;
  int m_nLayers;
  double* m_layerZ;
};

#endif /* SingleLayerTrackingEfficiencyPlot_hh */
