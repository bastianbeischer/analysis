#ifndef ChannelTriggerProbabilityPlot_hh
#define ChannelTriggerProbabilityPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class ChannelTriggerProbabilityPlot : public AnalysisPlot, public H1DPlot {
public:
  ChannelTriggerProbabilityPlot();
  virtual ~ChannelTriggerProbabilityPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
private:
  TH1D* m_normalizationHistogram;
};

#endif
