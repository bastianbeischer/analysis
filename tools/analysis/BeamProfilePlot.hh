#ifndef BeamProfilePlot_hh
#define BeamProfilePlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class BeamProfilePlot :
  public AnalysisPlot,
  public H2DProjectionPlot
{
  
public:
  enum Type {Horizontal, Vertical};

public:
  BeamProfilePlot(Type);
  ~BeamProfilePlot();
  
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

private:
  Type m_type;

};

#endif /* BeamProfilePlot_hh */
