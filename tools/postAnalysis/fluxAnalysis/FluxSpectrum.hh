#ifndef FluxSpectrum_hh
#define FluxSpectrum_hh

#include "SpectrumPlot.hh"

class SimulationFluxSelector;

class FluxSpectrum : public SpectrumPlot {
Q_OBJECT
public:
  FluxSpectrum();
  ~FluxSpectrum();
public slots:
  virtual void update();
private:
  void addSimulationSpectra();
  SimulationFluxSelector* m_simulationFluxSelector;
};

#endif
