#ifndef RESIDUALPLOTMC_HH
#define RESIDUALPLOTMC_HH

#include "ResidualPlot.hh"

class ResidualPlotMC : public ResidualPlot
{
public:
  ResidualPlotMC(AnalysisPlot::Topic topic, Layer* layer);
private:
  virtual Track* referenceTrack(const QVector<Hit*>&, Particle*, SimpleEvent*);
};

#endif // RESIDUALPLOTMC_HH
