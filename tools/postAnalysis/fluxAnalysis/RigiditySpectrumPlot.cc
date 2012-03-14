#include "RigiditySpectrumPlot.hh"

#include "FluxCalculation.hh"

RigiditySpectrumPlot::RigiditySpectrumPlot()
  : SpectrumPlot(&FluxCalculation::newRawSpectrum)
{
  setTitle("rigidity spectrum");
  setAxisTitle("|R| / GV", "");
}

RigiditySpectrumPlot::~RigiditySpectrumPlot()
{
}
