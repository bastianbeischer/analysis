#ifndef TotalEnergyDepositionPlot_hh
#define TotalEnergyDepositionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TotalEnergyDepositionPlot : public AnalysisPlot, public H1DPlot
{

public:
  TotalEnergyDepositionPlot();
  ~TotalEnergyDepositionPlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

};

#endif /* TotalEnergyDepositionPlott_hh */
