#ifndef MomentumSpectrumPlot_hh
#define MomentumSpectrumPlot_hh

#include "H1DPlot.hh"

class MomentumSpectrumPlot :
  public H1DPlot
{
  
public:
  MomentumSpectrumPlot();
  ~MomentumSpectrumPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  void finalize();

};

#endif /* MomentumSpectrumPlot_hh */
