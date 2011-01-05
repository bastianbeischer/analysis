#ifndef TRDDISTANCEWIRETOTRACKPLOT_HH
#define TRDDISTANCEWIRETOTRACKPLOT_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDDistanceWireToTrackPlot : public AnalysisPlot, public H1DPlot
{

public:
  TRDDistanceWireToTrackPlot(AnalysisPlot::Topic);
  ~TRDDistanceWireToTrackPlot();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();

};

#endif // TRDDISTANCEWIRETOTRACKPLOT_HH
