#ifndef SimulationFluxWidget_hh
#define SimulationFluxWidget_hh

#include "SimulationFluxSelector.hh"

class SimulationFluxWidget : public SimulationFluxSelector {
Q_OBJECT
public:
  SimulationFluxWidget(QWidget* = 0);
  ~SimulationFluxWidget();
private slots:
  void update();
};

#endif
