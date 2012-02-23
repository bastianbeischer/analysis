#include "SimulationFluxWidget.hh"

#include "Particle.hh"
#include "SimulationFluxReader.hh"
#include "SimulationFluxKey.hh"
#include "SelectionWidget.hh"

#include <TH1D.h>
#include <QList>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QComboBox>
#include <QPushButton>

SimulationFluxWidget::SimulationFluxWidget(QWidget* parent)
  : SimulationFluxSelector(2, parent)
{
}

SimulationFluxWidget::~SimulationFluxWidget()
{
  qDeleteAll(m_selectedHistograms);
}

void SimulationFluxWidget::update()
{
  if (m_inhibitUpdate)
    return;
  m_inhibitUpdate = true;
  qDeleteAll(m_selectedHistograms);
  m_selectedHistograms.clear();
  for(int iSelector = 0; iSelector < m_numberOfSelectors; ++iSelector) {
    QList<SimulationFluxKey> selectedHistograms;
    for (int iMenu = 0; iMenu < m_selectors[iSelector]->menu()->actions().size(); ++iMenu) {
      QAction* action = m_selectors[iSelector]->menu()->actions()[iMenu];
      if (action->isChecked()) {
        const QString& particleString = action->text();
        SimulationFluxKey::Location location = SimulationFluxKey::location(m_locationComboBox->currentText());
        SimulationFluxKey::Acceptance acceptance = SimulationFluxKey::acceptance(m_acceptanceComboBox->currentText());
        SimulationFluxKey::Source source = SimulationFluxKey::source(m_sourceComboBox->currentText());
        Enums::Particle particleType = SimulationFluxKey::particle(particleString);
        if (particleType == Enums::NoParticle)
          continue;
        double modulationParameter = SimulationFluxKey::modulationParameter(m_phiComboBox->currentText());
        bool isAlbedo = false;
        if (particleString.contains("albedo", Qt::CaseInsensitive))
          isAlbedo = true;
        const SimulationFluxKey key(location, acceptance, source, particleType, modulationParameter, isAlbedo);
        if (iSelector == 1)
          selectedHistograms << key;
        else
          m_selectedHistograms.append(SimulationFluxReader::instance()->spectrum(key));
      }
    }
    if (iSelector == 1 && selectedHistograms.size())
      m_selectedHistograms.append(SimulationFluxReader::instance()->spectrum(selectedHistograms));
  }
  m_inhibitUpdate = false;
  emit selectionChanged();
}
