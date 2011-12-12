#ifndef TrackFindingEfficiencyAzimuthCorrelation_hh
#define TrackFindingEfficiencyAzimuthCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "EfficiencyCorrectionSettings.hh"

#include <QVector>

class TH1D;

class TrackFindingEfficiencyAzimuthCorrelation : public AnalysisPlot, public H2DPlot {

public:
  TrackFindingEfficiencyAzimuthCorrelation(EfficiencyCorrectionSettings::FoldingType);
  ~TrackFindingEfficiencyAzimuthCorrelation();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();
  virtual void finalize();
private:
  TH2D* m_reconstructed;
  TH2D* m_total;
  bool isTriggerEvent(const QVector<Hit*>& clusters);
};

#endif
