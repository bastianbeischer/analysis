#include "RigidityFlux.hh"

#include "Track.hh"
#include "Plotter.hh"
#include "Constants.hh"
#include "RootQtWidget.hh"
#include "ParticleInformation.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "SimulationFluxWidget.hh"
#include "SettingsManager.hh"
#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TLegendEntry.h>
#include <TList.h>
#include <TPad.h>
#include <TF1.h>

#include <QVector>
#include <QDebug>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>

#include <cmath>
#include <vector>

RigidityFlux::RigidityFlux(Enums::ChargeSign type, int numberOfThreads, TH1D* particleHistogram, bool isAlbedo)
  : AnalysisPlot(Enums::MomentumReconstruction)
  , H1DPlot()
  , m_type(type)
  , m_measurementTimeCalculation(0)
  , m_fluxCalculation(0)
  , m_particleHistogram(particleHistogram)
  , m_particleHistogramMirrored(0)
  , m_isAlbedo(isAlbedo)
  , m_phiFit(0)
  , m_situation(Settings::Unknown)
  , m_simulationWidget(new SimulationFluxWidget)
{
  loadEfficiencies();
  QString title = "flux spectrum";
  m_measurementTimeCalculation = new MeasurementTimeCalculation(numberOfThreads);
  if (m_type == Enums::Negative) {
    title += " - negative";
    m_particleHistogramMirrored = Helpers::createMirroredHistogram(m_particleHistogram);
  } else if (m_type == Enums::Positive) {
    title += " - positive";
  } 
  if (m_particleHistogramMirrored) {
    m_fluxCalculation = new FluxCalculation(m_particleHistogramMirrored, m_measurementTimeCalculation->measurementTime());
  } else {
    m_fluxCalculation = new FluxCalculation(m_particleHistogram, m_measurementTimeCalculation->measurementTime());
  }
  setTitle(title);
  TH1D* histogram = m_fluxCalculation->fluxHistogram();
  histogram->GetXaxis()->SetMoreLogLabels(true);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  addHistogram(histogram, H1DPlot::P);

  const int nBins = histogram->GetNbinsX();
  if (nBins%2 == 0)
    m_nBinsNew = nBins / 2;
  else
    m_nBinsNew = (nBins - 1) / 2;
  
  if (nBins%2 == 0)
    m_nBinsStart = m_nBinsNew + 1;
  else
    m_nBinsStart = m_nBinsNew + 2;
  histogram->GetXaxis()->SetRange(m_nBinsStart, nBins);
  
  for(int i = 0; i < m_nBinsNew; i++) {
    TLatex* latex = new TLatex;
    latex->SetTextFont(42);
    latex->SetTextSize(0.02);
    latex->SetLineWidth(2);
    latex->SetTextAngle(45);
    latex->SetTextColor(kBlack);
    addLatex(latex);
  }

  TLegend* legend = new TLegend(.80, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->AddEntry(histogram, "Data", "p");
  addLegend(legend);

  const int low = histogram->GetXaxis()->FindBin(0.5);
  const int up = histogram->GetXaxis()->FindBin(10);
  histogram->GetXaxis()->SetRange(low, up);

  histogram->GetXaxis()->SetTitleOffset(1.2);
  histogram->GetYaxis()->SetTitleOffset(1.2);

  QWidget* secWidget = new QWidget();
  QVBoxLayout* layout = new QVBoxLayout(secWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_simulationWidget);
  connect(m_simulationWidget, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
  setSecondaryWidget(secWidget);
}

RigidityFlux::~RigidityFlux()
{
  if (m_measurementTimeCalculation) {
    delete m_measurementTimeCalculation;
    m_measurementTimeCalculation = 0;
  }
  if (m_fluxCalculation) {
    delete m_fluxCalculation;
    m_fluxCalculation = 0;
  }
  if (m_phiFit) {
    delete m_phiFit;
    m_phiFit = 0;
  }
}

void RigidityFlux::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  Q_ASSERT(settings);
  m_situation = settings->situation();
  m_measurementTimeCalculation->update(event);
}

void RigidityFlux::update()
{
  if (!m_phiFit && !m_isAlbedo && m_situation == Settings::KirunaFlight) {
    if (m_type == Enums::Negative) {
      m_phiFit = new SolarModulationFit(m_fluxCalculation->fluxHistogram(), Particle(Enums::Electron).pdgId());
      m_phiFit->setGamma(2.7);
    } else {
      m_phiFit = new SolarModulationFit(m_fluxCalculation->fluxHistogram(), Particle(Enums::Proton).pdgId());
    }
    addFunction(m_phiFit->fit());
    TLatex* J0Latex = RootPlot::newLatex(.4, .90);
    TLatex* gammaLatex = RootPlot::newLatex(.4, .85);
    TLatex* phiLatex = RootPlot::newLatex(.4, .80);
    TLatex* gammaClatex = RootPlot::newLatex(.4, .75);
    TLatex* rClatex = RootPlot::newLatex(.4, .70);
    addLatex(J0Latex);
    addLatex(gammaLatex);
    addLatex(phiLatex);
    addLatex(gammaClatex);
    addLatex(rClatex);
  }
  if (m_particleHistogramMirrored)
    Helpers::updateMirroredHistogram(m_particleHistogramMirrored, m_particleHistogram);
  m_fluxCalculation->update(m_measurementTimeCalculation->measurementTime());
  efficiencyCorrection();
  updateBinTitles();
  if (m_phiFit) {
    m_phiFit->fit();
    latex(m_nBinsNew + 0)->SetTitle(qPrintable(m_phiFit->J0Label()));
    latex(m_nBinsNew + 1)->SetTitle(qPrintable(m_phiFit->gammaLabel()));
    latex(m_nBinsNew + 2)->SetTitle(qPrintable(m_phiFit->phiLabel()));
    latex(m_nBinsNew + 3)->SetTitle(qPrintable(m_phiFit->gammaClabel()));
    latex(m_nBinsNew + 4)->SetTitle(qPrintable(m_phiFit->rClabel()));
  }
}

void RigidityFlux::updateBinTitles()
{
  for (int i = 0; i < m_nBinsNew; i++) {
    double bin = m_nBinsStart + i - 1;
    double x = xAxis()->GetBinCenterLog(bin + 1);
    double y = histogram()->GetBinContent(bin + 1);
    double value = m_particleHistogram->GetBinContent(bin);
    QString text = QString::number(value,'d',0);
    if (histogram(0)->GetXaxis()->GetFirst() > bin + 1 || bin + 1 >histogram(0)->GetXaxis()->GetLast())
      text = " ";
    latex(i)->SetX(x);
    latex(i)->SetY(y*1.1);
    latex(i)->SetTitle(qPrintable(text));
  }
}

void RigidityFlux::loadEfficiencies()
{
  const EfficiencyCorrectionSettings::FoldingType type = EfficiencyCorrectionSettings::Unfolded;
  if (m_type == Enums::Negative) {
    m_multiLayerEff = Helpers::createMirroredHistogram(EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency(type));
    m_trackFindingEff = Helpers::createMirroredHistogram(EfficiencyCorrectionSettings::instance()->trackFindingEfficiency(type));
  } else {
    m_multiLayerEff = EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency(type);
    m_trackFindingEff = EfficiencyCorrectionSettings::instance()->trackFindingEfficiency(type);
  }
}

void RigidityFlux::efficiencyCorrection()
{
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(histogram(), m_multiLayerEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(histogram(), m_trackFindingEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(histogram(), 0.999); //estimate for TOF trigger efficiency
}
  
void RigidityFlux::selectionChanged()
{
  while (legend()->GetListOfPrimitives()->GetEntries() > 1) {
    TLegendEntry* entry = (TLegendEntry*)legend()->GetListOfPrimitives()->At(1);
    legend()->GetListOfPrimitives()->Remove(entry);
    delete entry;
    entry = 0;
  }
  while(numberOfHistograms() > 1) {
    removeHistogram(1);
  }
  foreach(TH1D* histogram, m_simulationWidget->selectedHistograms()) {
    TH1D* newHisto = new TH1D(*histogram);
    if (!newHisto->GetSumw2())
      newHisto->Sumw2();
    addHistogram(newHisto, H1DPlot::HIST);
    legend()->AddEntry(newHisto, newHisto->GetTitle(), "l");
  }
  draw(Plotter::rootWidget()->GetCanvas());
  gPad->Modified();
  gPad->Update();
}
