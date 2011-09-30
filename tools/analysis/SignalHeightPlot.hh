#ifndef SignalHeightPlot_hh
#define SignalHeightPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class SignalHeightPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  SignalHeightPlot(AnalysisTopic, unsigned short);
  ~SignalHeightPlot();

  void processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const);

private:
  unsigned short m_id;
  
};

#endif /* SignalHeightPlot_hh */
