#ifndef TrackFindingEfficiency_hh
#define TrackFindingEfficiency_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "EfficiencyCorrectionSettings.hh"

#include <QVector>

class TH1D;

class TrackFindingEfficiency : public AnalysisPlot, public H1DPlot {
  
public:
  TrackFindingEfficiency(EfficiencyCorrectionSettings::FoldingType);
  ~TrackFindingEfficiency();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();
private:
  TH1D* m_reconstructed;
  TH1D* m_total;
  unsigned int m_nTotal;
  unsigned int m_nReconstructed;
  bool isTriggerEvent(const QVector<Hit*>&);
};

#endif
