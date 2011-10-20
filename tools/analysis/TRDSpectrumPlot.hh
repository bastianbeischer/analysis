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
  enum TRDSpectrumType{completeTRD, module, channel, layer};

  TRDSpectrumPlot(unsigned short = 0, TRDSpectrumType = completeTRD);
  ~TRDSpectrumPlot();

  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();

private:
  unsigned short m_id;
  TRDSpectrumType m_spectrumType;

};

#endif /* TRDSpectrumPlot_hh */
