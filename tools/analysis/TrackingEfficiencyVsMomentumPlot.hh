#ifndef TrackingEfficiencyVsMomentumPlot_hh
#define TrackingEfficiencyVsMomentumPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "Enums.hh"

class TCanvas;

class TrackingEfficiencyVsMomentumPlot : public AnalysisPlot, public H2DPlot
{
public:
  TrackingEfficiencyVsMomentumPlot(Enums::ChargeSigns);
  ~TrackingEfficiencyVsMomentumPlot();

  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void finalize();
private:
  Enums::ChargeSigns m_type;
};

#endif /* TrackingEfficiencyVsMomentumPlot_hh */
