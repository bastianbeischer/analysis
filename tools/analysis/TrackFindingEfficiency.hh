#ifndef TrackFindingEfficiency_hh
#define TrackFindingEfficiency_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QVector>

class TH1D;

class TrackFindingEfficiency : public AnalysisPlot, public H1DPlot {
  
public:
  TrackFindingEfficiency();
  ~TrackFindingEfficiency();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void update();
private:
  TH1D* m_reconstructed;
  TH1D* m_total;
  unsigned int m_nTotal;
  unsigned int m_nReconstructed;
  bool isTriggerEvent(const QVector<Hit*>& clusters);
};

#endif
