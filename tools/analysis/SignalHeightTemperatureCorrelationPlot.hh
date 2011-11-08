#ifndef SignalHeightTemperatureCorrelationPlot_hh
#define SignalHeightTemperatureCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class Particle;
class SimpleEvent;
class TH1D;

class SignalHeightTemperatureCorrelationPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  SignalHeightTemperatureCorrelationPlot(unsigned short sipmId);
  ~SignalHeightTemperatureCorrelationPlot();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
private:
  unsigned short m_sipmId;
  TH2D* m_histo;
  TH1D* m_normHisto;
};

#endif /* SignalHeightTemperatureCorrelationPlot_hh */
