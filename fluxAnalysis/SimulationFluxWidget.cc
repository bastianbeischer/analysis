#include "SimulationFluxWidget.hh"

#include <QDebug>

SimulationFluxWidget::SimulationFluxWidget(QWidget* parent)
  : SimulationFluxSelector(2, parent)
{
}

SimulationFluxWidget::~SimulationFluxWidget()
{
}

void SimulationFluxWidget::update()
{
  if (m_inhibitUpdate)
    return;
  m_inhibitUpdate = true;

  m_inhibitUpdate = false;
  emit selectionChanged();
}
