#ifndef AllTrackerLayersFlagEfficiency_hh
#define AllTrackerLayersFlagEfficiency_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;
class TCanvas;

class AllTrackerLayersFlagEfficiency : public AnalysisPlot, public H1DPlot
{
public:
  enum Type {Positive, Negative, All};
  AllTrackerLayersFlagEfficiency(Type type);
  ~AllTrackerLayersFlagEfficiency();

  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
  void finalize();
private:
  Type m_type;
  TH1D* m_afterCutHisto;
  TH1D* m_normHisto;
};

#endif /* AllTrackerLayersFlagEfficiency_hh */
