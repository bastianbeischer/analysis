#ifndef TRDClustersOnTrackPlot_hh
#define TRDClustersOnTrackPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDClustersOnTrackPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  TRDClustersOnTrackPlot(Enums::AnalysisTopic);
  ~TRDClustersOnTrackPlot();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void update();

};

#endif /* TRDClustersOnTrackPlot_hh */
