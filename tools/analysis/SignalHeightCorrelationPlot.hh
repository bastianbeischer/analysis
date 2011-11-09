#ifndef SignalHeightCorrelationPlot_hh
#define SignalHeightCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QString>

class Particle;
class SimpleEvent;
class TH1D;

class SignalHeightCorrelationPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  enum SensorType {Undefined, Time, Temperature};
  SignalHeightCorrelationPlot(unsigned short id, SensorType, TH2D*);
  ~SignalHeightCorrelationPlot();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
private:
  unsigned short m_id;
  SensorType m_type;
  TH2D* m_histo;
  TH1D* m_normHisto;
};

#endif
