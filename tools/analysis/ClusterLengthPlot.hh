#ifndef ClusterLengthPlot_hh
#define ClusterLengthPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class ClusterLengthPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  ClusterLengthPlot(AnalysisTopic, unsigned short);
  ~ClusterLengthPlot();

  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();

private:
  unsigned short m_id;
  
};

#endif /* ClusterLengthPlot_hh */
