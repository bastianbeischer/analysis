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
  enum CorrelationType {Undefined, Time, Temperature, Rigidity};
  SignalHeightCorrelationPlot(unsigned short id, CorrelationType, TH2D*);
  SignalHeightCorrelationPlot(CorrelationType, TH2D*);
  ~SignalHeightCorrelationPlot();
  void processEvent(const AnalyzedEvent*);
  void update();
private:
  unsigned short m_id;
  CorrelationType m_type;
  TH2D* m_histo;
  TH1D* m_normHisto;
};

#endif
