#ifndef SimulationFluxRatioWidget_hh
#define SimulationFluxRatioWidget_hh

#include "SimulationFluxSelector.hh"

class SimulationFluxRatioWidget : public SimulationFluxSelector {
Q_OBJECT
public:
  SimulationFluxRatioWidget(QWidget* = 0);
  ~SimulationFluxRatioWidget();
private slots:
  void update();
};

#endif
