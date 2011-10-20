#ifndef SignalHeightPlot_hh
#define SignalHeightPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class SignalHeightPlot : public AnalysisPlot, public H1DPlot {
public:
  SignalHeightPlot(Enums::AnalysisTopic, unsigned short);
  ~SignalHeightPlot();
  void processEvent(const AnalyzedEvent*);
private:
  unsigned short m_id;
};

#endif /* SignalHeightPlot_hh */
