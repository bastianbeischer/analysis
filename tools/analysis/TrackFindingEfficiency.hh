#ifndef TrackFindingEfficiency_hh
#define TrackFindingEfficiency_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Track.hh"
#include "Hit.hh"

#include <TH2D.h>
#include <TLatex.h>
#include <TCanvas.h>

#include <QString>
#include <QDateTime>

class Hit;
class Particle;
class SimpleEvent;

class TrackFindingEfficiency : public AnalysisPlot, public H1DPlot {
  
public:
  enum Type {
    All,
    Positive,
    Negative,
  };

  TrackFindingEfficiency(Type type);
  ~TrackFindingEfficiency();
  
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void update();
  
private:
  Type m_type;
  TH1D* m_reconstructed;
  TH1D* m_total;
  bool hasTrdTrack(const QVector<Hit*>&);
  unsigned int m_nTotal;
  unsigned int m_nReconstructed;
};

#endif
