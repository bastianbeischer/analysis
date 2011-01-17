#ifndef TotalEnergyDepositionPlot_hh
#define TotalEnergyDepositionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TotalEnergyDepositionPlot : public AnalysisPlot, public H1DPlot
{

public:
  TotalEnergyDepositionPlot(double lowerMomentum = -10, double upperMomentum = 10);
  ~TotalEnergyDepositionPlot();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);

private:
  double m_lowerMomentum;
  double m_upperMomentum;


};

#endif /* TotalEnergyDepositionPlott_hh */
