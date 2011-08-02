#ifndef SimulationFlux_hh
#define SimulationFlux_hh

#include "Particle.hh"
#include "SimulationFluxKey.hh"

#include <TH1D.h>
#include <TH2D.h>

class SimulationFlux {
	
public:
  SimulationFlux(SimulationFluxKey key, TH2D* h2Spectrum);
	~SimulationFlux();

  TH2D* h2Spectrum();
  TH1D* spectrum();

private:
  SimulationFluxKey m_key;
  TH2D* m_h2Spectrum;

  void scaleYToBinWidth(TH2D* histogram);
  TH1D* projectionY(TH2D* histogram, double xMin, double xMax);
  void scaleToBinWidth(TH1D* histogram);

};


#endif
