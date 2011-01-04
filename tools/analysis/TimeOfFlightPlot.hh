#ifndef TimeOfFlightPlot_hh
#define TimeOfFlightPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeOfFlightPlot : public AnalysisPlot, public H1DPlot {
public:
  TimeOfFlightPlot();
  virtual ~TimeOfFlightPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void update();
};

#endif
