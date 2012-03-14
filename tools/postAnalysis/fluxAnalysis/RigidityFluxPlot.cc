#include "RigidityFluxPlot.hh"

#include "FluxCalculation.hh"

RigidityFluxPlot::RigidityFluxPlot()
  : SpectrumPlot(&FluxCalculation::newFluxSpectrum)
{
  setTitle("flux spectrum");
  setAxisTitle("|R| / GV", "flux");
}

RigidityFluxPlot::~RigidityFluxPlot()
{
}
