#include "FluxSpectrum.hh"

#include "FluxCalculation.hh"

FluxSpectrum::FluxSpectrum()
  : SpectrumPlot(&FluxCalculation::newFluxSpectrum)
{
  setTitle("flux spectrum");
  setAxisTitle("|R| / GV", "flux");
}

FluxSpectrum::~FluxSpectrum()
{
}
