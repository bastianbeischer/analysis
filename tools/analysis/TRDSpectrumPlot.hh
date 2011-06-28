#ifndef TRDSpectrumPlot_hh
#define TRDSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include <TF1.h>
#include <TMarker.h>

#include <QString>

class TRDSpectrumPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  enum TRDSpectrumType{completeTRD, module, channel};

  TRDSpectrumPlot(unsigned short = 0, TRDSpectrumType = completeTRD);
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

  TMarker* m_fitRangeMarker_lower;
  TMarker* m_fitRangeMarker_upper;

};

#endif /* TRDSpectrumPlot_hh */
