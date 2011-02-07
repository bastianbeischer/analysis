#ifndef MomentumSpectrumPlot_hh
#define MomentumSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class MomentumSpectrumPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  enum Type {All=0, Positive=1, Negative=2, Inverted=3};

public:
  MomentumSpectrumPlot(Type range = All);
  ~MomentumSpectrumPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  void update();

private:
  Type m_type;

};

#endif /* MomentumSpectrumPlot_hh */
