#ifndef TimeDifferencePlot_hh
#define TimeDifferencePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeDifferencePlot : public AnalysisPlot, public H2DPlot {
  public:
    TimeDifferencePlot();
    virtual ~TimeDifferencePlot();
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, TrackSelection* = 0, SimpleEvent* = 0);
    virtual void finalize();
};

#endif
