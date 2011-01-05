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

  TRDSpectrumPlot(AnalysisPlot::Topic, unsigned short, TRDSpectrumType, double lowerMomentum = -10, double upperMomentum = 10);
  ~TRDSpectrumPlot();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void update();

  TF1* landauFit(){return m_landauFit;}

private:
  unsigned short m_id;
  TRDSpectrumType m_spectrumType;

  TF1* m_landauFit;
  const double m_landauFitRange_lower;
  const double m_landauFitRange_upper;
  const double m_lowerMomentum;
  const double m_upperMomentum;

  TMarker* m_fitRangeMarker_lower;
  TMarker* m_fitRangeMarker_upper;

};

#endif /* TRDSpectrumPlot_hh */
