#ifndef TRDSpectrumPlot_hh
#define TRDSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include <TF1.h>
#include <TMarker.h>

class TRDSpectrumPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  enum TRDSpectrumType{completeTRD, module, channel};
  enum CherenkovCut{bothAbove, bothBelow, c1AboveC2Below, c1BelowC2Above, none};

  TRDSpectrumPlot(unsigned short, TRDSpectrumType, double lowerMomentum = -100, double upperMomentum = 100, CherenkovCut cCut = none, double c1Limit = 200, double c2Limit = 200);
  ~TRDSpectrumPlot();

  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void update();
  virtual void draw(TCanvas*);

private:
  unsigned short m_id;
  TRDSpectrumType m_spectrumType;

  TF1* m_landauFit;
  const double m_landauFitRange_lower;
  const double m_landauFitRange_upper;
  const double m_lowerMomentum;
  const double m_upperMomentum;
  const CherenkovCut m_cherenkovCut;
  const double m_cherenkov1Limit;
  const double m_cherenkov2Limit;

  TMarker* m_fitRangeMarker_lower;
  TMarker* m_fitRangeMarker_upper;

};

#endif /* TRDSpectrumPlot_hh */
