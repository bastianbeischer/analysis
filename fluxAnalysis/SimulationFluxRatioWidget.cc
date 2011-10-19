#include "SimulationFluxRatioWidget.hh"

#include "Particle.hh"
#include "SimulationFluxReader.hh"

#include <TH1D.h>
#include <QList>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QComboBox>
#include <SimulationFluxKey.hh>

SimulationFluxRatioWidget::SimulationFluxRatioWidget(QWidget* parent)
  : SimulationFluxSelector(2, parent)
{
}

SimulationFluxRatioWidget::~SimulationFluxRatioWidget()
{
  qDeleteAll(m_selectedHistograms);
}

void SimulationFluxRatioWidget::update()
{
  if (m_inhibitUpdate)
    return;
  m_inhibitUpdate = true;
  qDeleteAll(m_selectedHistograms);
  m_selectedHistograms.clear();
  QList<SimulationFluxKey> numerator;
  QList<SimulationFluxKey> denominator;
  for(int iSelector = 0; iSelector < m_numberOfSelectors; ++iSelector) {
    QList<SimulationFluxKey> selectedHistograms;
    for (int iMenu = 0; iMenu < m_buttonMenus[iSelector]->actions().size(); ++iMenu) {
      QAction* action = m_buttonMenus[iSelector]->actions()[iMenu];
      if (action->isChecked()) {
        const QString& particleString = action->text();
        SimulationFluxKey::Location location = SimulationFluxKey::location(m_locationComboBox->currentText());
        SimulationFluxKey::Acceptance acceptance = SimulationFluxKey::acceptance(m_acceptanceComboBox->currentText());
        SimulationFluxKey::Source source = SimulationFluxKey::source(m_sourceComboBox->currentText());
        Enums::Particle particleType = SimulationFluxKey::particle(particleString);
        double modulationParameter = SimulationFluxKey::modulationParameter(m_phiComboBox->currentText());
        bool isAlbedo = false;
        if (particleString.contains("albedo", Qt::CaseInsensitive))
          isAlbedo = true;
        const SimulationFluxKey key(location, acceptance, source, particleType, modulationParameter, isAlbedo);
        if (iSelector == 0)
          numerator << key;
        else
          denominator << key;
      }
    }
  }
  if (numerator.size() && denominator.size()) {
    TH1D* numeratorHistogram = SimulationFluxReader::instance()->spectrum(numerator);
    TH1D* denominatorHistogram = SimulationFluxReader::instance()->spectrum(denominator);
    numeratorHistogram->Divide(denominatorHistogram);
    m_selectedHistograms.append(numeratorHistogram);
    delete denominatorHistogram;
  }
  m_inhibitUpdate = false;
  emit selectionChanged();
}
