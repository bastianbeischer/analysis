#ifndef SimulationFlux_hh
#define SimulationFlux_hh

#include "SimulationFluxKey.hh"

#include <TH1D.h>
#include <TH2D.h>

class SimulationFlux {
public:
  SimulationFlux(SimulationFluxKey key, TH2D* h2Spectrum);
  ~SimulationFlux();
  TH1D* spectrum();
private:
  void scaleYToBinWidth(TH2D* histogram);
  TH1D* projectionY(TH2D* histogram, double xMin, double xMax);
  void scaleToBinWidth(TH1D* histogram);
  SimulationFluxKey m_key;
  TH2D* m_h2Spectrum;
};


#endif
