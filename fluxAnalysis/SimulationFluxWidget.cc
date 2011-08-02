#include "SimulationFluxWidget.hh"

#include "SimulationFluxReader.hh"
#include "RootQtWidget.hh"
#include "H1DPlot.hh"

#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TPad.h>

#include <QDebug>
#include <QLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QCheckBox>
#include <QList>
#include <QAction>

SimulationFluxWidget::SimulationFluxWidget(H1DPlot* plot, TCanvas* canvas, QWidget* parent) :
QWidget(parent),
m_plot(plot),
m_canvas(canvas)
{
  m_reader = 0;
  m_activated = false;

  m_layout = new QHBoxLayout(this);

  m_activateButton = new QPushButton("Simulation:");
  m_layout->addWidget(m_activateButton);
  m_activateButton->setCheckable(true);
  m_activateButton->setChecked(false);

  m_locationBox = new QComboBox();
  m_layout->addWidget(m_locationBox);

  m_layout->addWidget(new QLabel("Phi/MV:"));
  m_phiBox = new QComboBox();
  m_layout->addWidget(m_phiBox);

  m_layout->addWidget(new QLabel("Origin:"));
  m_sourceBox = new QComboBox();
  m_layout->addWidget(m_sourceBox);

  m_layout->addWidget(new QLabel("Show:"));
  m_individualParticleButton = new QPushButton("Individual Fluxes");
  m_individualParticleMenu  = new QMenu(this);
  m_layout->addWidget(m_individualParticleButton);
  m_individualParticleButton->setMenu(m_individualParticleMenu);

  m_sumParticleButton = new QPushButton("Sum Flux");
  m_sumParticleMenu  = new QMenu(this);
  m_layout->addWidget(m_sumParticleButton);
  m_sumParticleButton->setMenu(m_sumParticleMenu);

  m_layout->addWidget(new QLabel("Select all:"));
  m_positiveButton = new QPushButton("+");
  m_layout->addWidget(m_positiveButton);

  m_negativeButton = new QPushButton("-");
  m_layout->addWidget(m_negativeButton);

  m_clearButton = new QPushButton("Clear");
  m_layout->addWidget(m_clearButton);

  deactivateControls();

  connect(m_activateButton, SIGNAL(toggled(bool)), this, SLOT(activationSlot(bool)));
  connect(m_locationBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setLocation(QString)));
  connect(m_phiBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPhi(QString)));
  connect(m_sourceBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setSource(QString)));
  connect(m_clearButton, SIGNAL(clicked()), this, SLOT(clear()));
  connect(m_positiveButton, SIGNAL(clicked()), this, SLOT(selectPositive()));
  connect(m_negativeButton, SIGNAL(clicked()), this, SLOT(selectNegative()));
}

SimulationFluxWidget::~SimulationFluxWidget()
{
  deactivateControls();

  if (m_reader) {
    delete m_reader;
    m_reader = 0;
  }

  QLayoutItem *item;
  while ((item = m_layout->takeAt(0))) {
    delete item;
    item = 0;
  }
  delete m_layout;
  m_layout = 0;
}

void SimulationFluxWidget::setLocation(QString name)
{
  SimulationFluxReader::Location location = SimulationFluxReader::location(name);
  if (m_location != location) {
    clear();
    m_location = location;
    delete m_reader;
    m_reader = new SimulationFluxReader(m_location);
    setPhiBox();
    setPhi(m_phiBox->currentText());
    updateFluxes();
  }
}

void SimulationFluxWidget::setSource(QString name)
{
  m_source = SimulationFluxKey::source(name);
  setIndividualParticles();
  updateFluxes();
}

void SimulationFluxWidget::setPhi(QString name)
{
  m_phi = name.toDouble();
  updateFluxes();
}

void SimulationFluxWidget::setIndividualParticles()
{
  m_individualParticleMenu->clear();
  m_individualParticles.clear();
  m_individualParticleActions.clear();
  for(int isAlbedo = 0; isAlbedo < 2; isAlbedo++) {
    foreach(Particle::Type type, SimulationFluxKey::allParticles()) {
      SimulationFluxKey currentKey = SimulationFluxKey(m_phi, type, isAlbedo, m_source);
      m_individualParticles.push_back(currentKey);
      QAction* action = new QAction(currentKey.name(),this);
      action->setCheckable(true);
      m_individualParticleActions.push_back(action);
      m_individualParticleMenu->addAction(action);
      connect(action, SIGNAL(triggered()), m_individualParticleButton, SLOT(showMenu()));

      connect(action, SIGNAL(changed()), this, SLOT(updateIndividualParticles()));
      connect(action, SIGNAL(triggered()), this, SLOT(updateIndividualParticles()));
    }
  }
  updateFluxes();
}

void SimulationFluxWidget::setSumParticles()
{
  m_sumParticleMenu->clear();
  m_sumParticles.clear();
  m_sumParticleActions.clear();
  for(int isAlbedo = 0; isAlbedo < 2; isAlbedo++) {
    foreach(Particle::Type type, SimulationFluxKey::allParticles()) {
      SimulationFluxKey currentKey = SimulationFluxKey(m_phi, type, isAlbedo, m_source);
      m_sumParticles.push_back(currentKey);
      QAction* action = new QAction(currentKey.name(),this);
      action->setCheckable(true);
      m_sumParticleActions.push_back(action);
      m_sumParticleMenu->addAction(action);
      connect(action, SIGNAL(triggered()), m_sumParticleButton, SLOT(showMenu()));

      connect(action, SIGNAL(changed()), this, SLOT(updateSumParticles()));
      connect(action, SIGNAL(triggered()), this, SLOT(updateSumParticles()));
    }
  }
  updateFluxes();
}

void SimulationFluxWidget::setPhiBox()
{
  m_phiBox->clear();
  foreach(double phi, m_reader->phis()) {
    m_phiBox->addItem(QString::number(phi));
  }
  m_phi = m_phiBox->currentText().toDouble();
}

void SimulationFluxWidget::updateIndividualParticles()
{
  m_individualParticlesChecked.clear();
  for(int i = 0; i < m_individualParticleActions.size(); i++) {
    QAction* action = m_individualParticleActions.at(i);
    if (action->isChecked()) {
      SimulationFluxKey currentKey = m_individualParticles.at(i);
      currentKey.setPhi(m_phi);
      currentKey.setSource(m_source);
      m_individualParticlesChecked.push_back(currentKey);
    }
  }
  updateFluxes();
}

void SimulationFluxWidget::updateSumParticles()
{
  m_sumParticlesChecked.clear();
  for(int i = 0; i < m_sumParticleActions.size(); i++) {
    QAction* action = m_sumParticleActions.at(i);
    if (action->isChecked()) {
      SimulationFluxKey currentKey = m_sumParticles.at(i);
      currentKey.setPhi(m_phi);
      currentKey.setSource(m_source);
      m_sumParticlesChecked.push_back(currentKey);
    }
  }
  updateFluxes();
}

void SimulationFluxWidget::updateFluxes()
{
  foreach(TH1D* flux, m_fluxes) {
    delete flux;
  }
  m_fluxes.clear();
  foreach(SimulationFluxKey particle, m_individualParticlesChecked) {
    particle.setPhi(m_phi);
    particle.setSource(m_source);
    TH1D* histogram = m_reader->spectrum(particle);
    histogram->SetLineStyle(2);
    if (particle.isAlbedo()) {
      histogram->SetLineStyle(3);
    }
    m_fluxes.push_back(histogram);
  }
  if ( m_sumParticlesChecked.size() != 0) {
    for(int i = 0; i < m_sumParticlesChecked.size(); ++i) {
      m_sumParticlesChecked[i].setPhi(m_phi);
      m_sumParticlesChecked[i].setSource(m_source);
    }
    m_fluxes.push_back(m_reader->spectrum(m_sumParticlesChecked));
  }
  updatePlot();
}

void SimulationFluxWidget::updatePlot()
{
  if (m_plot) {
    if (!m_plot->drawn()) {
      return;
    }
    clearPlot();

    m_canvas->cd();

    foreach(TH1D* histogram, m_fluxes) {
      TH1D* newHisto = new TH1D(*histogram);
      if (!newHisto->GetSumw2())
        newHisto->Sumw2();
      m_plot->addHistogram(newHisto, H1DPlot::HIST);
      if (m_plot->hasLegend())
        m_plot->legend()->AddEntry(newHisto, newHisto->GetTitle(), "l");
    }
    m_plot->histogram(0)->GetXaxis()->SetMoreLogLabels(true);
    m_plot->draw(m_canvas);
    m_plot->update();

    gPad->Update();
    gPad->Modified();
  }
}

void SimulationFluxWidget::clearLegend() {
  if(m_plot->hasLegend()) {
    while (m_plot->legend()->GetListOfPrimitives()->GetEntries() > 1) {
      TLegendEntry* entry = (TLegendEntry*)m_plot->legend()->GetListOfPrimitives()->At(1);
      m_plot->legend()->GetListOfPrimitives()->Remove(entry);
      delete entry;
      entry = 0;
    }
  }
}

void SimulationFluxWidget::clearPlot() {
  clearLegend();

  while(m_plot->numberOfHistograms() > 1) {
    m_plot->removeHistogram(1);
  }

  if (m_plot) {
    m_canvas->cd();

    m_plot->update();
    gPad->Update();
    gPad->Modified();
  }
}

void SimulationFluxWidget::clear() {
  foreach(TH1D* flux, m_fluxes) {
    delete flux;
  }
  m_fluxes.clear();
  m_individualParticlesChecked.clear();
  m_sumParticlesChecked.clear();

  foreach(SimulationFluxKey simParticle, m_individualParticles) {
    int index = m_individualParticles.indexOf(simParticle);
    m_individualParticleActions[index]->setChecked(false);
    m_sumParticleActions[index]->setChecked(false);
  }

  clearPlot();
}

void SimulationFluxWidget::selectPositive()
{
  foreach(SimulationFluxKey simParticle, m_individualParticles) {
    if (simParticle.isAlbedo()) {
      continue;
    }
    Particle particle(simParticle.type());
    if (particle.charge() > 0) {
      int index = m_individualParticles.indexOf(simParticle);
      if (!m_individualParticlesChecked.contains(simParticle)) {
        m_individualParticlesChecked.push_back(simParticle);
        m_individualParticleActions[index]->setChecked(true);
      }
      if (!m_sumParticlesChecked.contains(simParticle)) {
        m_sumParticlesChecked.push_back(simParticle);
        m_sumParticleActions[index]->setChecked(true);
      }
    }
  }
  updateFluxes();
}

void SimulationFluxWidget::selectNegative()
{
  foreach(SimulationFluxKey simParticle, m_individualParticles) {
    if (simParticle.isAlbedo()) {
      continue;
    }
    Particle particle(simParticle.type());
    if (particle.charge() < 0) {
      int index = m_individualParticles.indexOf(simParticle);
      if (!m_individualParticlesChecked.contains(simParticle)) {
        m_individualParticlesChecked.push_back(simParticle);
        m_individualParticleActions[index]->setChecked(true);
      }
      if (!m_sumParticlesChecked.contains(simParticle)) {
        m_sumParticlesChecked.push_back(simParticle);
        m_sumParticleActions[index]->setChecked(true);
      }
    }
  }
  updateFluxes();
}

void SimulationFluxWidget::activationSlot(bool isChecked)
{
  if (isChecked) {
    activateControls();
  } else {
    deactivateControls();
  }

}

void SimulationFluxWidget::activateControls()
{
  if (!m_activated) {

    SimulationFluxReader::reset();

    foreach(SimulationFluxReader::Location location, SimulationFluxReader::allLocations()) {
      m_locationBox->addItem(SimulationFluxReader::locationName(location));
    }

    if (m_locationBox->count() == 0) {
      qDebug("Wont activate if there are no files!");
      m_activateButton->setChecked(false);
      return;
    }

    foreach(SimulationFluxKey::Source source, SimulationFluxKey::allSources()) {
      m_sourceBox->addItem(SimulationFluxKey::sourceName(source));
    }

    m_location = SimulationFluxReader::location(m_locationBox->currentText());

    m_reader = new SimulationFluxReader(m_location);
    setPhiBox();
    setPhi(m_phiBox->currentText());

    setSource(m_sourceBox->currentText());
    setIndividualParticles();
    setSumParticles();

    gStyle->SetErrorX(0);
    m_activated = true;

  }
  QList<QPushButton*> allPButtons = this->findChildren<QPushButton*>();
  foreach(QPushButton* button, allPButtons) {
    button->setEnabled(true);
  }
  QList<QComboBox*> allCBoxes = this->findChildren<QComboBox*>();
  foreach(QComboBox* box, allCBoxes) {
    box->setEnabled(true);
  }
}

void SimulationFluxWidget::deactivateControls()
{
  QList<QPushButton*> allPButtons = this->findChildren<QPushButton*>();
  foreach(QPushButton* button, allPButtons) {
    button->setEnabled(false);
  }
  QList<QComboBox*> allCBoxes = this->findChildren<QComboBox*>();
  foreach(QComboBox* box, allCBoxes) {
    box->setEnabled(false);
  }
  m_activated = false;
  m_activateButton->setEnabled(true);
}
