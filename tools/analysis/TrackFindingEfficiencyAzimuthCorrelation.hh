#ifndef TrackFindingEfficiencyAzimuthCorrelation_hh
#define TrackFindingEfficiencyAzimuthCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>

class TH1D;

class TrackFindingEfficiencyAzimuthCorrelation : public AnalysisPlot, public H2DPlot {
public:
  TrackFindingEfficiencyAzimuthCorrelation(bool fineBinned);
  ~TrackFindingEfficiencyAzimuthCorrelation();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();
private:
  TH2D* m_reconstructed;
  TH2D* m_total;
  bool isTriggerEvent(const QVector<Hit*>& clusters);
};

#endif
