#ifndef MCTRDSpectrumPlot_hh
#define MCTRDSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "TRDSpectrumPlot.hh"
#include <TF1.h>
#include <TMarker.h>
#include <QMap>

class TCanvas;

class MCTRDSpectrumPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  MCTRDSpectrumPlot(unsigned short = 0, TRDSpectrumPlot::TRDSpectrumType = TRDSpectrumPlot::completeTRD);
  ~MCTRDSpectrumPlot();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const);
  virtual void finalize();
  virtual void draw(TCanvas* canvas);

private:
  QMap <int, TH1D*> m_spectrumMap;
  int m_colorCounter;
  unsigned short m_id;
  TRDSpectrumPlot::TRDSpectrumType m_spectrumType;
};

#endif /* MCTRDSpectrumPlot_hh */
