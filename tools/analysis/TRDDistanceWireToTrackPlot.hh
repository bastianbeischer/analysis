#ifndef TRDDISTANCEWIRETOTRACKPLOT_HH
#define TRDDISTANCEWIRETOTRACKPLOT_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDDistanceWireToTrackPlot : public AnalysisPlot, public H1DPlot
{

public:
  TRDDistanceWireToTrackPlot(Enums::AnalysisTopic);
  ~TRDDistanceWireToTrackPlot();

  virtual void processEvent(const AnalyzedEvent*);

};

#endif // TRDDISTANCEWIRETOTRACKPLOT_HH
