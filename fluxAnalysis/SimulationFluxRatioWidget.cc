#include "SimulationFluxRatioWidget.hh"

#include "SimulationFluxReader.hh"
#include "H1DPlot.hh"
#include "RootQtWidget.hh"

#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TH1.h>
#include <TPad.h>

#include <QDebug>
#include <QLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QCheckBox>
#include <QList>
#include <QAction>

SimulationFluxRatioWidget::SimulationFluxRatioWidget(H1DPlot* plot, TCanvas* canvas, QWidget* parent) :
QWidget(parent),
m_plot(plot),
m_canvas(canvas)
{
  m_nPrevHistograms = m_plot->numberOfHistograms();
  m_nPrevLegends = m_plot->legend()->GetListOfPrimitives()->GetEntries();

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

  m_layout->addWidget(new QLabel("Ratio:"));

  m_numeratorSumParticleButton = new QPushButton("Numerator");
  m_numeratorSumParticleMenu  = new QMenu(this);
  m_layout->addWidget(m_numeratorSumParticleButton);
  m_numeratorSumParticleButton->setMenu(m_numeratorSumParticleMenu);

  m_denominatorSumParticleButton = new QPushButton("Denominator");
  m_denominatorSumParticleMenu  = new QMenu(this);
  m_layout->addWidget(m_denominatorSumParticleButton);
  m_denominatorSumParticleButton->setMenu(m_denominatorSumParticleMenu);

  m_clearButton = new QPushButton("Clear");
  m_layout->addWidget(m_clearButton);

  deactivateControls();

  connect(m_activateButton, SIGNAL(toggled(bool)), this, SLOT(activationSlot(bool)));
  connect(m_locationBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setLocation(QString)));
  connect(m_phiBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPhi(QString)));
  connect(m_sourceBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setSource(QString)));
  connect(m_clearButton, SIGNAL(clicked()), this, SLOT(clear()));
}

SimulationFluxRatioWidget::~SimulationFluxRatioWidget()
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

void SimulationFluxRatioWidget::setLocation(QString name)
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

void SimulationFluxRatioWidget::setSource(QString name)
{
  m_source = SimulationFluxKey::source(name);
  updateFluxes();
}

void SimulationFluxRatioWidget::setPhi(QString name)
{
  m_phi = name.toDouble();
  updateFluxes();
}

void SimulationFluxRatioWidget::setNumeratorSumParticles()
{
  m_numeratorSumParticleMenu->clear();
  m_numeratorSumParticles.clear();
  m_numeratorSumParticleActions.clear();
  for(int isAlbedo = 0; isAlbedo < 2; isAlbedo++) {
    foreach(Particle::Type type, SimulationFluxKey::allParticles()) {
      SimulationFluxKey currentParticle = SimulationFluxKey(m_phi, type, isAlbedo, m_source);
      m_numeratorSumParticles.push_back(currentParticle);
      QAction* action = new QAction(currentParticle.name(),this);
      action->setCheckable(true);
      m_numeratorSumParticleActions.push_back(action);
      m_numeratorSumParticleMenu->addAction(action);
      connect(action, SIGNAL(triggered()), m_numeratorSumParticleButton, SLOT(showMenu()));

      connect(action, SIGNAL(changed()), this, SLOT(updateNumeratorSumParticles()));
      connect(action, SIGNAL(triggered()), this, SLOT(updateNumeratorSumParticles()));
    }
  }
  updateFluxes();
}

void SimulationFluxRatioWidget::setDenominatorSumParticles()
{
  m_denominatorSumParticleMenu->clear();
  m_denominatorSumParticles.clear();
  m_denominatorSumParticleActions.clear();
  for(int isAlbedo = 0; isAlbedo < 2; isAlbedo++) {
    foreach(Particle::Type type, SimulationFluxKey::allParticles()) {
      SimulationFluxKey currentParticle = SimulationFluxKey(m_phi, type, isAlbedo, m_source);
      m_denominatorSumParticles.push_back(currentParticle);
      QAction* action = new QAction(currentParticle.name(),this);
      action->setCheckable(true);
      m_denominatorSumParticleActions.push_back(action);
      m_denominatorSumParticleMenu->addAction(action);
      connect(action, SIGNAL(triggered()), m_denominatorSumParticleButton, SLOT(showMenu()));

      connect(action, SIGNAL(changed()), this, SLOT(updateDenominatorSumParticles()));
      connect(action, SIGNAL(triggered()), this, SLOT(updateDenominatorSumParticles()));
    }
  }
  updateFluxes();
}

void SimulationFluxRatioWidget::setPhiBox()
{
  m_phiBox->clear();
  foreach(double phi, m_reader->phis()) {
    m_phiBox->addItem(QString::number(phi));
  }
  m_phi = m_phiBox->currentText().toDouble();
}

void SimulationFluxRatioWidget::updateNumeratorSumParticles()
{
  m_numeratorSumParticlesChecked.clear();
  for(int i = 0; i < m_numeratorSumParticleActions.size(); i++) {
    QAction* action = m_numeratorSumParticleActions.at(i);
    if (action->isChecked()) {
      SimulationFluxKey currentParticle = m_numeratorSumParticles.at(i);
      currentParticle.setPhi(m_phi);
      currentParticle.setSource(m_source);
      m_numeratorSumParticlesChecked.push_back(currentParticle);
    }
  }
  updateFluxes();
}

void SimulationFluxRatioWidget::updateDenominatorSumParticles()
{
  m_denominatorSumParticlesChecked.clear();
  for(int i = 0; i < m_denominatorSumParticleActions.size(); i++) {
    QAction* action = m_denominatorSumParticleActions.at(i);
    if (action->isChecked()) {
      SimulationFluxKey currentParticle = m_denominatorSumParticles.at(i);
      currentParticle.setPhi(m_phi);
      currentParticle.setSource(m_source);
      m_denominatorSumParticlesChecked.push_back(currentParticle);
    }
  }
  updateFluxes();
}

void SimulationFluxRatioWidget::updateFluxes()
{
  foreach(TH1D* flux, m_ratios) {
    delete flux;
    flux= 0;
  }
  m_ratios.clear();

  if (m_numeratorSumParticlesChecked.size() != 0 && m_denominatorSumParticlesChecked.size() != 0) {
    TH1D* numerator = 0;
    TH1D* denominator = 0;
    TH1D* ratio = 0;

    for(int i = 0; i < m_numeratorSumParticlesChecked.size(); ++i) {
      m_numeratorSumParticlesChecked[i].setPhi(m_phi);
      m_numeratorSumParticlesChecked[i].setSource(m_source);
    }
    for(int i = 0; i < m_denominatorSumParticlesChecked.size(); ++i) {
      m_denominatorSumParticlesChecked[i].setPhi(m_phi);
      m_denominatorSumParticlesChecked[i].setSource(m_source);
    }

    numerator = (m_reader->spectrum(m_numeratorSumParticlesChecked));
    denominator = (m_reader->spectrum(m_denominatorSumParticlesChecked));

    ratio = new TH1D(*numerator);
    if (!ratio->GetSumw2())
      ratio->Sumw2();
    ratio->Divide(numerator, denominator);

    delete numerator;
    numerator = 0;
    delete denominator;
    denominator = 0;

    m_ratios.push_back(ratio);
  }

  updatePlot();
}

void SimulationFluxRatioWidget::updatePlot()
{
  if (m_plot) {
    if (!m_plot->drawn()) {
      return;
    }
    clearPlot();

    m_canvas->cd();

    foreach(TH1D* histogram, m_ratios) {
      TH1D* newHisto = new TH1D(*histogram);
      if (!newHisto->GetSumw2())
        newHisto->Sumw2();
      m_plot->addHistogram(newHisto, H1DPlot::HIST);
      m_plot->legend()->AddEntry(newHisto, "Simulation", "l");
    }

    m_plot->histogram(0)->GetXaxis()->SetMoreLogLabels(true);
    m_plot->draw(m_canvas);
    m_plot->update();
    gPad->Update();
    gPad->Modified();
  }
}

void SimulationFluxRatioWidget::clearLegend() {
  while (m_plot->legend()->GetListOfPrimitives()->GetEntries() > 1) {
    TLegendEntry* entry = (TLegendEntry*)m_plot->legend()->GetListOfPrimitives()->At(1);
    m_plot->legend()->GetListOfPrimitives()->Remove(entry);
    delete entry;
    entry = 0;
  }
}

void SimulationFluxRatioWidget::clearPlot() {
  clearLegend();

  while(m_plot->numberOfHistograms() > m_nPrevHistograms) {
    m_plot->removeHistogram(m_nPrevHistograms);
  }

  if (m_plot) {
    m_canvas->cd();

    m_plot->update();
    gPad->Update();
    gPad->Modified();
  }
}

void SimulationFluxRatioWidget::clear() {
  foreach(TH1D* flux, m_ratios) {
    delete flux;
    flux= 0;
  }
  m_ratios.clear();
  m_numeratorSumParticlesChecked.clear();
  m_denominatorSumParticlesChecked.clear();

  foreach(SimulationFluxKey simParticle, m_numeratorSumParticles) {
    int index = m_numeratorSumParticles.indexOf(simParticle);
    m_numeratorSumParticleActions[index]->setChecked(false);
    m_denominatorSumParticleActions[index]->setChecked(false);
  }

  clearPlot();
}

void SimulationFluxRatioWidget::activationSlot(bool isChecked)
{
  if (isChecked) {
    activateControls();
  } else {
    deactivateControls();
  }

}

void SimulationFluxRatioWidget::activateControls()
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
    setNumeratorSumParticles();
    setDenominatorSumParticles();

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

void SimulationFluxRatioWidget::deactivateControls()
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
