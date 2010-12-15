#ifndef SignalHeightPlot_hh
#define SignalHeightPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class SignalHeightPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  SignalHeightPlot(AnalysisPlot::Topic, unsigned short);
  ~SignalHeightPlot();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);

private:
  unsigned short m_id;
  
};

#endif /* SignalHeightPlot_hh */
