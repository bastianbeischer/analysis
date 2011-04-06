#ifndef Chi2VsMomentumPlot_hh
#define Chi2VsMomentumPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TH1D;
class TCanvas;

class Chi2VsMomentumPlot :
  public AnalysisPlot,
  public H2DPlot
{
  
public:
  Chi2VsMomentumPlot();
  ~Chi2VsMomentumPlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

};

#endif /* Chi2VsMomentumPlot_hh */
