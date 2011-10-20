#ifndef AllTrackerLayersFlagEfficiency_hh
#define AllTrackerLayersFlagEfficiency_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "EfficiencyCorrectionSettings.hh"

class TH1D;
class TCanvas;

class AllTrackerLayersFlagEfficiency : public AnalysisPlot, public H1DPlot
{
public:
  AllTrackerLayersFlagEfficiency(EfficiencyCorrectionSettings::FoldingType);
  ~AllTrackerLayersFlagEfficiency();
  void processEvent(const AnalyzedEvent*);
  void update();
private:
  TH1D* m_afterCutHisto;
  TH1D* m_normHisto;
};

#endif /* AllTrackerLayersFlagEfficiency_hh */
