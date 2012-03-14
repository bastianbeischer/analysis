#include "RawSpectrum.hh"

#include "FluxCalculation.hh"

RawSpectrum::RawSpectrum()
  : SpectrumPlot(&FluxCalculation::newRawSpectrum)
{
  setTitle("raw spectrum");
  setAxisTitle("|R| / GV", "");
}

RawSpectrum::~RawSpectrum()
{
}
