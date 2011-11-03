#ifndef SignalHeightTimeCorrelationPlot_hh
#define SignalHeightTimeCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QDateTime>

class Particle;
class SimpleEvent;
class TH1D;

class SignalHeightTimeCorrelationPlot :
  public AnalysisPlot,
  public H2DProjectionPlot
{
  
public:
  SignalHeightTimeCorrelationPlot(unsigned short sipmId, QDateTime first, QDateTime last);
  ~SignalHeightTimeCorrelationPlot();
  
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();

private:
  unsigned short m_sipmId;
  TH2D* m_histo;
  TH1D* m_normHisto;

};

#endif /* SignalHeightTimeCorrelationPlot_hh */
