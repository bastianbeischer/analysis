#ifndef ChannelTriggerProbabilityPlot_hh
#define ChannelTriggerProbabilityPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class ChannelTriggerProbabilityPlot : public AnalysisPlot, public H1DPlot {
public:
  ChannelTriggerProbabilityPlot();
  virtual ~ChannelTriggerProbabilityPlot();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
  virtual void draw(TCanvas* canvas);
private:
  TH1D* m_normalizationHistogram;
};

#endif
