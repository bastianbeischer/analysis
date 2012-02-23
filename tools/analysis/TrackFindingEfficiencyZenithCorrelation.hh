#ifndef TrackFindingEfficiencyZenithCorrelation_hh
#define TrackFindingEfficiencyZenithCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>

class TH1D;

class TrackFindingEfficiencyZenithCorrelation : public AnalysisPlot, public H2DPlot {
public:
  TrackFindingEfficiencyZenithCorrelation(bool fineBinned);
  ~TrackFindingEfficiencyZenithCorrelation();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();
  virtual void finalize();
private:
  TH2D* m_reconstructed;
  TH2D* m_total;
  bool isTriggerEvent(const QVector<Hit*>& clusters);
};

#endif
