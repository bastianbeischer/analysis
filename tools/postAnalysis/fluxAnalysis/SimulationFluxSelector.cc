#include "SimulationFluxSelector.hh"

#include "SimulationFluxReader.hh"
#include "ParticleProperties.hh"
#include "RootQtWidget.hh"
#include "H1DPlot.hh"
#include "SelectionWidget.hh"

#include <QDebug>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

SimulationFluxSelector::SimulationFluxSelector(const QStringList& list, QWidget* parent)
  : QWidget(parent)
  , m_locationComboBox(new QComboBox)
  , m_acceptanceComboBox(new QComboBox)
  , m_sourceComboBox(new QComboBox)
  , m_modulationParameterComboBox(new QComboBox)
  , m_selectors()
{
  QGridLayout* layout = new QGridLayout();
  layout->addWidget(m_locationComboBox, 0, 0);
  layout->addWidget(m_acceptanceComboBox, 0, 1);
  layout->addWidget(m_sourceComboBox, 0, 2);
  layout->addWidget(m_modulationParameterComboBox, 0, 3);

  for (int i = 0; i < list.count(); ++i) {
    layout->addWidget(new QLabel(list[i]), i + 1, 0, 1, 2, Qt::AlignVCenter);
    ParticleSelector* selector = 0;
    selector = new ParticleSelector("particles");
    layout->addWidget(selector, i + 1, 2, Qt::AlignVCenter);
    m_selectors.append(selector);
    selector = new ParticleSelector("albedos");
    layout->addWidget(selector, i + 1, 3, Qt::AlignVCenter);
    m_selectors.append(selector);
  }

  setLayout(layout);

  fillLocationComboBox();
  locationChanged();
  connect(m_locationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(locationChanged()));
}

SimulationFluxSelector::~SimulationFluxSelector()
{
}

QVector<SimulationFluxKey> SimulationFluxSelector::keys(int i) const
{
  Q_ASSERT(0 <= 2*i && 2*i+1 < m_selectors.count());
  SimulationFluxKey::Location location = SimulationFluxKey::location(m_locationComboBox->currentText());
  SimulationFluxKey::Acceptance acceptance = SimulationFluxKey::acceptance(m_acceptanceComboBox->currentText());
  SimulationFluxKey::Source source = SimulationFluxKey::source(m_sourceComboBox->currentText());
  double modulation = SimulationFluxKey::modulationParameter(m_modulationParameterComboBox->currentText());
  QVector<Enums::Particle> particleVector = m_selectors[2*i]->selectedElementsVector();
  QVector<Enums::Particle> albedoParticleVector = m_selectors[2*i+1]->selectedElementsVector();

  QVector<SimulationFluxKey> keys;
  foreach (Enums::Particle particle, particleVector)
    keys.append(SimulationFluxKey(location, acceptance, source, particle, modulation, false));
  foreach (Enums::Particle particle, albedoParticleVector)
    keys.append(SimulationFluxKey(location, acceptance, source, particle, modulation, true));
  return keys;
}

void SimulationFluxSelector::locationChanged()
{
  disconnect(m_acceptanceComboBox, SIGNAL(currentIndexChanged(int)), 0, 0);
  disconnect(m_sourceComboBox, SIGNAL(currentIndexChanged(int)), 0, 0);
  disconnect(m_modulationParameterComboBox, SIGNAL(currentIndexChanged(int)), 0, 0);
  foreach (SelectionWidget* selector, m_selectors)
    disconnect(selector, SIGNAL(selectionChanged()), 0, 0);

  fillAcceptanceComboBox();
  fillSourceComboBox();
  fillModulationParameterComboBox();
  fillSelectors();

  connect(m_acceptanceComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectionChanged()));
  connect(m_sourceComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectionChanged()));
  connect(m_modulationParameterComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectionChanged()));
  foreach (SelectionWidget* selector, m_selectors)
    connect(selector, SIGNAL(selectionChanged()), this, SIGNAL(selectionChanged()));

  emit selectionChanged();
}

void SimulationFluxSelector::fillLocationComboBox()
{
  const QVector<SimulationFluxKey::Location>& locations = SimulationFluxReader::instance()->locations();
  foreach (SimulationFluxKey::Location location, locations)
    m_locationComboBox->addItem(SimulationFluxKey::locationName(location));
  int defaultIndex = m_locationComboBox->findText(SimulationFluxKey::locationName(SimulationFluxKey::Flight));
  if (0 <= defaultIndex)
    m_locationComboBox->setCurrentIndex(defaultIndex);
}

void SimulationFluxSelector::fillAcceptanceComboBox()
{
  m_acceptanceComboBox->clear();
  SimulationFluxKey::Location location = SimulationFluxKey::location(m_locationComboBox->currentText());
  const QVector<SimulationFluxKey::Acceptance>& acceptances = SimulationFluxReader::instance()->acceptances(location);
  foreach (SimulationFluxKey::Acceptance acceptance, acceptances)
    m_acceptanceComboBox->addItem(SimulationFluxKey::acceptanceName(acceptance));
}

void SimulationFluxSelector::fillSourceComboBox()
{
  m_sourceComboBox->clear();
  SimulationFluxKey::Location location = SimulationFluxKey::location(m_locationComboBox->currentText());
  const QVector<SimulationFluxKey::Source>& sources = SimulationFluxReader::instance()->sources(location);
  foreach (SimulationFluxKey::Source source, sources)
    m_sourceComboBox->addItem(SimulationFluxKey::sourceName(source));
}

void SimulationFluxSelector::fillSelectors()
{
  SimulationFluxKey::Location location = SimulationFluxKey::location(m_locationComboBox->currentText());
  const QVector<Enums::Particle>& particles = SimulationFluxReader::instance()->particles(location);
  foreach (ParticleSelector* selector, m_selectors) {
    selector->clear();
    foreach (Enums::Particle particle, particles) {
      for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it) {
        if (it.key() == particle) {
          selector->addElement(it);
          break;
        }
      }
    }
  }
}

void SimulationFluxSelector::fillModulationParameterComboBox()
{
  m_modulationParameterComboBox->clear();
  SimulationFluxKey::Location location = SimulationFluxKey::location(m_locationComboBox->currentText());
  const QVector<double>& phis = SimulationFluxReader::instance()->modulationParameters(location);
  foreach (double phi, phis)
    m_modulationParameterComboBox->addItem(SimulationFluxKey::modulationParameterName(phi));
  int defaultIndex = m_modulationParameterComboBox->findText(SimulationFluxKey::modulationParameterName(550));
  if (0 <= defaultIndex)
    m_modulationParameterComboBox->setCurrentIndex(defaultIndex);
}
