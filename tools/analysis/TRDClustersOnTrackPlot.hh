#ifndef TRDClustersOnTrackPlot_hh
#define TRDClustersOnTrackPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDClustersOnTrackPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  TRDClustersOnTrackPlot(AnalysisPlot::Topic);
  ~TRDClustersOnTrackPlot();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();

};

#endif /* TRDClustersOnTrackPlot_hh */
