#ifndef AllTrackerLayersFlagEfficiency_hh
#define AllTrackerLayersFlagEfficiency_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;
class TCanvas;

class AllTrackerLayersFlagEfficiency : public AnalysisPlot, public H1DPlot
{
public:
  AllTrackerLayersFlagEfficiency(bool fineBinned);
  ~AllTrackerLayersFlagEfficiency();
  void processEvent(const AnalyzedEvent*);
  void update();
private:
  TH1D* m_passedCutHistogram;
  TH1D* m_totalHistogram;
};

#endif /* AllTrackerLayersFlagEfficiency_hh */
