#ifndef MomentumSpectrumPlot_hh
#define MomentumSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class MomentumSpectrumPlot :
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  enum Range {All=0, Positive=1, Negative=2};

public:
  MomentumSpectrumPlot(Range range = All);
  ~MomentumSpectrumPlot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);

private:
  Range m_range;

};

#endif /* MomentumSpectrumPlot_hh */
