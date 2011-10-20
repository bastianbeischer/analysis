#ifndef RESIDUALPLOTMC_HH
#define RESIDUALPLOTMC_HH

#include "ResidualPlot.hh"

class ResidualPlotMC : public ResidualPlot
{
public:
  ResidualPlotMC(Enums::AnalysisTopic topic, Layer* layer);
private:
  virtual Track* referenceTrack(const AnalyzedEvent*);
};

#endif // RESIDUALPLOTMC_HH
