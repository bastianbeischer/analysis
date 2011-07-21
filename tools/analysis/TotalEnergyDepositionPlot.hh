#ifndef TotalEnergyDepositionPlot_hh
#define TotalEnergyDepositionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TotalEnergyDepositionPlot : public AnalysisPlot, public H1DPlot
{

public:
  TotalEnergyDepositionPlot();
  ~TotalEnergyDepositionPlot();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);

};

#endif /* TotalEnergyDepositionPlott_hh */
