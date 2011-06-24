#ifndef MCTRDSpectrumPlot_hh
#define MCTRDSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include <TF1.h>
#include <TMarker.h>
#include <QMap>

class TCanvas;

class MCTRDSpectrumPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  enum TRDSpectrumType{completeTRD, module, channel};

  MCTRDSpectrumPlot(unsigned short = 0, TRDSpectrumType = completeTRD);
  ~MCTRDSpectrumPlot();

  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void draw(TCanvas* canvas);

private:
  QMap <int, TH1D*> m_spectrumMap;
  int m_colorCounter;
  unsigned short m_id;
  TRDSpectrumType m_spectrumType;
};

#endif /* MCTRDSpectrumPlot_hh */
