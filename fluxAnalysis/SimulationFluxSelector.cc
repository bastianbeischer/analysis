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
  , m_inhibitClear(false)
  , m_selectedHistograms()
{
  m_layout = new QHBoxLayout(this);
  m_layout->setContentsMargins(0, 0, 0, 0);
  m_activateButton = new QPushButton("activate");
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
  connect(m_locationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

  m_acceptanceComboBox = new QComboBox(this);
  m_layout->addWidget(m_acceptanceComboBox);
  fillAceptanceComboBox();
  connect(m_acceptanceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  
  m_sourceComboBox = new QComboBox(this);
  m_layout->addWidget(m_sourceComboBox);
  fillSourceComboBox();
  connect(m_sourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  
  for (int i = 0; i < m_numberOfSelectors; ++i) {
    QMenu* menu = new QMenu(this);
    QPushButton* button = new QPushButton("particles", this);
    button->setMenu(menu);
    m_layout->addWidget(button);

    if (i < 2) {
      QString actionName = "all positive";
      QAction* action = new QAction(actionName, this);
      action->setCheckable(false);
      button->menu()->addAction(action);
      if (i == 0)
        connect(action, SIGNAL(triggered()), this, SLOT(selectPositive0()));
      else if (i == 1)
        connect(action, SIGNAL(triggered()), this, SLOT(selectPositive1()));

      actionName = "all negative";
      action = new QAction(actionName, this);
      action->setCheckable(false);
      button->menu()->addAction(action);
      if (i == 0)
        connect(action, SIGNAL(triggered()), this, SLOT(selectNegative0()));
      else if (i == 1)
        connect(action, SIGNAL(triggered()), this, SLOT(selectNegative1()));

      button->menu()->addSeparator ();
    }

    fillMenu(button);
    m_buttons.append(button);
    m_buttonMenus.append(menu);
    connect(button, SIGNAL(clicked()), this, SLOT(update()));
  }

  m_phiComboBox = new QComboBox(this);
  m_layout->addWidget(m_phiComboBox);
  fillPhiComboBox();
  connect(m_phiComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
}

void SimulationFluxSelector::clear()
{
  if (m_inhibitClear)
    return;
  m_inhibitClear = true;
  for(int iSelector = 0; iSelector < m_numberOfSelectors; ++iSelector) {
    for (int iMenu = 0; iMenu < m_buttonMenus[iSelector]->actions().size(); ++iMenu) {
      m_buttonMenus[iSelector]->actions()[iMenu]->setChecked(false);
    }
  }
  m_inhibitClear = false;
  emit selectionChanged();
}

void SimulationFluxSelector::selectPositive0()
{
  selectActions(0, true);
}

void SimulationFluxSelector::selectNegative0()
{
  selectActions(0, false);
}

void SimulationFluxSelector::selectPositive1()
{
  selectActions(1, true);
}

void SimulationFluxSelector::selectNegative1()
{
  selectActions(1, false);
}

void SimulationFluxSelector::fillLocationComboBox()
{
  const QVector<SimulationFluxKey::Location>& locations = SimulationFluxReader::instance()->locations();
  foreach (SimulationFluxKey::Location location, locations) {
    m_locationComboBox->addItem(SimulationFluxKey::locationName(location));
  }
}

void SimulationFluxSelector::fillAceptanceComboBox()
{
  const QVector<SimulationFluxKey::Acceptance>& acceptances = SimulationFluxReader::instance()->acceptances();
  foreach (SimulationFluxKey::Acceptance acceptance, acceptances) {
    m_acceptanceComboBox->addItem(SimulationFluxKey::acceptanceName(acceptance));
  }
}

void SimulationFluxSelector::fillSourceComboBox()
{
  const QVector<SimulationFluxKey::Source>& sources = SimulationFluxReader::instance()->sources();
  foreach (SimulationFluxKey::Source source, sources) {
    m_sourceComboBox->addItem(SimulationFluxKey::sourceName(source));
  }
}

void SimulationFluxSelector::fillMenu(QPushButton* button)
{
  for (int isAlbedo = 0; isAlbedo < 2; ++isAlbedo) {
    foreach (Particle::Type particleType, SimulationFluxReader::instance()->particles()) {
      QString actionName = SimulationFluxKey::particleName(particleType);
      if (isAlbedo)
        actionName.prepend("albedo ");
      QAction* action = new QAction(actionName, this);
      action->setCheckable(true);
      button->menu()->addAction(action);
      connect(action, SIGNAL(triggered()), button, SLOT(showMenu()));
      connect(action, SIGNAL(changed()), this, SLOT(update()));
    }
    if (isAlbedo == 0)
      button->menu()->addSeparator ();
  }
}

void SimulationFluxSelector::fillPhiComboBox()
{
  const QVector<double>& phis = SimulationFluxReader::instance()->modulationParameters();
  foreach (double phi, phis) {
    m_phiComboBox->addItem(SimulationFluxKey::modulationParameterName(phi));
  }
  int defaultIndex = m_phiComboBox->findText(SimulationFluxKey::modulationParameterName(550));
  m_phiComboBox->setCurrentIndex(defaultIndex);
}

void SimulationFluxSelector::selectActions(int iSelector, bool positive)
{
  for (int iMenu = 0; iMenu < m_buttonMenus[iSelector]->actions().size(); ++iMenu) {
    QAction* action = m_buttonMenus[iSelector]->actions()[iMenu];
    const QString& particleString = action->text();
    Particle::Type particleType = SimulationFluxKey::particle(particleString);
    if (particleType == Particle::Unknown)
      continue;
    bool isAlbedo = false;
    if (particleString.contains("albedo", Qt::CaseInsensitive))
      isAlbedo = true;
    if (!isAlbedo && ((Particle(particleType).charge() > 0 && positive) || (Particle(particleType).charge() < 0 && !positive)))
      action->setChecked(true);
  }
  emit selectionChanged();
}
