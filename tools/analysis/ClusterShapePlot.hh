#ifndef ClusterShapePlot_hh
#define ClusterShapePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TGaxis;

class ClusterShapePlot : 
  public AnalysisPlot,
  public H1DPlot
{
  
public:
  ClusterShapePlot(unsigned short);
  ~ClusterShapePlot();
  
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void draw(TCanvas*);
  void update();
  void finalize();

private:
  unsigned short m_id;
  unsigned int   m_eventCounter;

  TGaxis*        m_eventCountAxis;
  
};

#endif /* ClusterShapePlot_hh */
