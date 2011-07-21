#ifndef RESIDUALPLOTMC_HH
#define RESIDUALPLOTMC_HH

#include "ResidualPlot.hh"

class ResidualPlotMC : public ResidualPlot
{
public:
  ResidualPlotMC(AnalysisPlot::Topic topic, Layer* layer);
private:
  virtual Track* referenceTrack(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
};

#endif // RESIDUALPLOTMC_HH
