#ifndef TrackFindingEfficiencyZenithCorrelation_hh
#define TrackFindingEfficiencyZenithCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "EfficiencyCorrectionSettings.hh"

#include <QVector>

class TH1D;

class TrackFindingEfficiencyZenithCorrelation : public AnalysisPlot, public H2DPlot {
  
public:
  TrackFindingEfficiencyZenithCorrelation(EfficiencyCorrectionSettings::FoldingType);
  ~TrackFindingEfficiencyZenithCorrelation();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void update();
  virtual void finalize();
private:
  TH2D* m_reconstructed;
  TH2D* m_total;
  bool isTriggerEvent(const QVector<Hit*>& clusters);
};

#endif
