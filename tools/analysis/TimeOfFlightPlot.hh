#ifndef TimeOfFlightPlot_hh
#define TimeOfFlightPlot_hh

#include "H1DPlot.hh"

class TimeOfFlightPlot : public H1DPlot {
  public:
    TimeOfFlightPlot();
    virtual ~TimeOfFlightPlot();
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
    virtual void finalize();
};

#endif
