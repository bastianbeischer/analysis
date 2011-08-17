#include "SimulationFluxRatioWidget.hh"

#include <QDebug>

SimulationFluxRatioWidget::SimulationFluxRatioWidget(QWidget* parent)
  : SimulationFluxSelector(2, parent)
{
}

SimulationFluxRatioWidget::~SimulationFluxRatioWidget()
{
}

void SimulationFluxRatioWidget::update()
{
  if (m_inhibitUpdate)
    return;
  m_inhibitUpdate = true;

  m_inhibitUpdate = false;
}
