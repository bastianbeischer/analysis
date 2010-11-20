#ifndef MomentumSpectrumPlot_hh
#define MomentumSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class MomentumSpectrumPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  MomentumSpectrumPlot();
  ~MomentumSpectrumPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, TrackSelection* = 0, SimpleEvent* = 0);
  void finalize();

};

#endif /* MomentumSpectrumPlot_hh */
