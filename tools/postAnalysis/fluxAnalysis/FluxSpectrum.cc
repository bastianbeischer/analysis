#include "FluxSpectrum.hh"

#include "FluxCalculation.hh"

FluxSpectrum::FluxSpectrum()
  : SpectrumPlot(&FluxCalculation::newFluxSpectrum)
{
  setTitle("flux spectrum");
  setAxisTitle("|R| / GV", "flux / GV^{-1}m^{-2}s^{-1}sr^{-1}");
}

FluxSpectrum::~FluxSpectrum()
{
}
