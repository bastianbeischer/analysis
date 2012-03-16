#ifndef AllTrackerLayersFlagEfficiency_hh
#define AllTrackerLayersFlagEfficiency_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;
class TCanvas;

class AllTrackerLayersFlagEfficiency : public AnalysisPlot, public H1DPlot
{
public:
  AllTrackerLayersFlagEfficiency(Enums::Particles, bool fineBinned);
  ~AllTrackerLayersFlagEfficiency();
  void processEvent(const AnalyzedEvent*);
  void update();
private:
  QVector<Enums::Particle> m_particles;
  QVector<TH1D*> m_passedCutHistograms;
  QVector<TH1D*> m_totalHistograms;
};

#endif /* AllTrackerLayersFlagEfficiency_hh */
