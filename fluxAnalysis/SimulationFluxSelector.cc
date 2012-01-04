#include "SimulationFluxSelector.hh"

#include "SimulationFluxReader.hh"
#include "RootQtWidget.hh"
#include "H1DPlot.hh"

#include <QDebug>
#include <QLayout>
#include <QComboBox>
#include <QStringList>
#include <QPushButton>
#include <QAction>
#include <QMenu>

SimulationFluxSelector::SimulationFluxSelector(int numberOfSelectors, QWidget* parent)
  : QWidget(parent)
  , m_numberOfSelectors(numberOfSelectors)
  , m_layout(0)
  , m_activateButton(0)
  , m_locationComboBox(0)
  , m_acceptanceComboBox(0)
  , m_sourceComboBox(0)
  , m_buttons()
  , m_buttonMenus()
  , m_phiComboBox(0)
  , m_inhibitUpdate(false)
  , m_selectedHistograms()
  , m_location(SimulationFluxKey::UndefinedLocation)
{
  m_layout = new QHBoxLayout(this);
  m_layout->setContentsMargins(0, 0, 0, 0);
  m_activateButton = new QPushButton("activate simulation");
  m_layout->addWidget(m_activateButton);
  m_layout->addStretch();
  connect(m_activateButton, SIGNAL(clicked()), this, SLOT(activate()));
}

SimulationFluxSelector::~SimulationFluxSelector()
{
}

void SimulationFluxSelector::activate()
{
  m_activateButton->disconnect();
  connect(m_activateButton, SIGNAL(clicked()), this, SLOT(clear()));
  m_activateButton->setText("clear");

  m_locationComboBox = new QComboBox(this);
  m_layout->addWidget(m_locationComboBox);
  fillLocationComboBox();
  connect(m_locationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(locationChanged()));

  m_acceptanceComboBox = new QComboBox(this);
  m_layout->addWidget(m_acceptanceComboBox);
  connect(m_acceptanceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

  m_sourceComboBox = new QComboBox(this);
  m_layout->addWidget(m_sourceComboBox);
  connect(m_sourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

  for (int i = 0; i < m_numberOfSelectors; ++i) {
    QMenu* menu = new QMenu(this);
    QPushButton* button = new QPushButton("particles", this);
    button->setMenu(menu);
    m_layout->addWidget(button);
    m_buttons.append(button);
    m_buttonMenus.append(menu);
    connect(button, SIGNAL(clicked()), this, SLOT(update()));
  }

  m_phiComboBox = new QComboBox(this);
  m_layout->addWidget(m_phiComboBox);
  connect(m_phiComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  locationChanged();
}

void SimulationFluxSelector::clear()
{
  m_inhibitUpdate = true;
  for(int iSelector = 0; iSelector < m_numberOfSelectors; ++iSelector) {
    for (int iMenu = 0; iMenu < m_buttonMenus[iSelector]->actions().size(); ++iMenu) {
      m_buttonMenus[iSelector]->actions()[iMenu]->setChecked(false);
    }
  }
  m_inhibitUpdate = false;
  update();
}

void SimulationFluxSelector::selectPositive()
{
  checkParticles(static_cast<QAction*>(sender()), true);
}

void SimulationFluxSelector::selectNegative()
{
  checkParticles(static_cast<QAction*>(sender()), false);
}

void SimulationFluxSelector::locationChanged()
{
  if (m_inhibitUpdate)
    return;
  m_inhibitUpdate = true;
  m_acceptanceComboBox->clear();
  m_sourceComboBox->clear();
  m_phiComboBox->clear();
  for (int i = 0; i < m_numberOfSelectors; ++i) {
    foreach(QAction* action, m_buttonMenus[i]->actions())
      action->disconnect();
    m_buttonMenus[i]->clear();
  }
  m_location = SimulationFluxKey::location(m_locationComboBox->currentText());
  fillAceptanceComboBox();
  fillSourceComboBox();
  for (int i = 0; i < m_numberOfSelectors; ++i)
    fillMenu(m_buttons[i]);
  fillPhiComboBox();
  m_inhibitUpdate = false;
  update();
}

void SimulationFluxSelector::fillLocationComboBox()
{
  const QVector<SimulationFluxKey::Location>& locations = SimulationFluxReader::instance()->locations();
  foreach (SimulationFluxKey::Location location, locations) {
    m_locationComboBox->addItem(SimulationFluxKey::locationName(location));
  }
  int defaultIndex = m_locationComboBox->findText(SimulationFluxKey::locationName(SimulationFluxKey::Flight));
  m_locationComboBox->setCurrentIndex(defaultIndex);
}

void SimulationFluxSelector::fillAceptanceComboBox()
{
  const QVector<SimulationFluxKey::Acceptance>& acceptances = SimulationFluxReader::instance()->acceptances(m_location);
  foreach (SimulationFluxKey::Acceptance acceptance, acceptances) {
    m_acceptanceComboBox->addItem(SimulationFluxKey::acceptanceName(acceptance));
  }
}

void SimulationFluxSelector::fillSourceComboBox()
{
  const QVector<SimulationFluxKey::Source>& sources = SimulationFluxReader::instance()->sources(m_location);
  foreach (SimulationFluxKey::Source source, sources) {
    m_sourceComboBox->addItem(SimulationFluxKey::sourceName(source));
  }
}

void SimulationFluxSelector::fillMenu(QPushButton* button)
{
  QMenu* menu = button->menu();
  QAction* action = 0;
  action = new QAction("positive", menu);
  connect(action, SIGNAL(triggered()), this, SLOT(selectPositive()));
  menu->addAction(action);
  action = new QAction("negative", menu);
  connect(action, SIGNAL(triggered()), this, SLOT(selectNegative()));
  menu->addAction(action);
  menu->addSeparator();
  for (int isAlbedo = 0; isAlbedo < 2; ++isAlbedo) {
    foreach (Enums::Particle particleType, SimulationFluxReader::instance()->particles(m_location)) {
      QString actionName = SimulationFluxKey::particleName(particleType);
      if (isAlbedo)
        actionName.prepend("albedo ");
      action = new QAction(actionName, menu);
      action->setCheckable(true);
      menu->addAction(action);
      connect(action, SIGNAL(triggered()), button, SLOT(showMenu()));
      connect(action, SIGNAL(changed()), this, SLOT(update()));
    }
    if (isAlbedo == 0)
      menu->addSeparator();
  }
}

void SimulationFluxSelector::fillPhiComboBox()
{
  const QVector<double>& phis = SimulationFluxReader::instance()->modulationParameters(m_location);
  foreach (double phi, phis) {
    m_phiComboBox->addItem(SimulationFluxKey::modulationParameterName(phi));
  }
  int defaultIndex = m_phiComboBox->findText(SimulationFluxKey::modulationParameterName(550));
  m_phiComboBox->setCurrentIndex(defaultIndex);
}

void SimulationFluxSelector::checkParticles(QAction* action, bool positive)
{
  m_inhibitUpdate = true;
  foreach (QAction* action, static_cast<QMenu*>(action->parent())->actions()) {
    const QString& particleString = action->text();
    Enums::Particle particleType = SimulationFluxKey::particle(particleString);
    if (particleType == Enums::NoParticle)
      continue;
    bool isAlbedo = false;
    if (particleString.contains("albedo", Qt::CaseInsensitive))
      isAlbedo = true;
    if (!isAlbedo && ((Particle(particleType).charge() > 0 && positive) || (Particle(particleType).charge() < 0 && !positive)))
      action->setChecked(true);
  }
  m_inhibitUpdate = false;
  update();
}
