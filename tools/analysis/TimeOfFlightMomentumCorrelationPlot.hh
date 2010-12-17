#ifndef TimeOfFlightMomentumCorrelationPlot_hh
#define TimeOfFlightMomentumCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;
class TF1;

class TimeOfFlightMomentumCorrelationPlot : public AnalysisPlot, public H2DPlot {
public:
  explicit TimeOfFlightMomentumCorrelationPlot();
  virtual ~TimeOfFlightMomentumCorrelationPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void draw(TCanvas*);
private:
  QVector<TF1*> m_functions;
};

#endif
