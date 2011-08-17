#include "RigidityFlux.hh"

#include "Track.hh"
#include "Plotter.hh"
#include "Constants.hh"
#include "RootQtWidget.hh"
#include "ParticleInformation.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "SimulationFluxWidget.hh"
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

#include <cmath>
#include <vector>

RigidityFlux::RigidityFlux(Type type, const QDateTime& first, const QDateTime& last, TH1D* particleHistogram)
  : AnalysisPlot(AnalysisPlot::MomentumReconstruction)
  , H1DPlot()
  , m_type(type)
  , m_effCor(0)
  , m_particleHistogram(particleHistogram)
  , m_particleHistogramMirrored(0)
{
  QString title = "flux spectrum";
  m_measurementTimeCalculation = new MeasurementTimeCalculation(first, last);
  if (m_type == Negative) {
    title += " - negative";
    m_particleHistogramMirrored = Helpers::createMirroredHistogram(m_particleHistogram);
  } else if (m_type == Positive) {
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
    double bin = m_nBinsStart -1 + i;
    addLatex(new TLatex(m_fluxCalculation->binTitle(bin+1)));
  }

  TLegend* legend = new TLegend(.80, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->AddEntry(histogram, "Data", "p");
  addLegend(legend);

  m_phiFit = new SolarModulationFit(histogram);
  addFunction(m_phiFit->fit());
  TLatex* gammaLatex = RootPlot::newLatex(.4, .88);
  TLatex* phiLatex = RootPlot::newLatex(.4, .83);
  addLatex(gammaLatex);
  addLatex(phiLatex);

  SimulationFluxWidget* secWidget = new SimulationFluxWidget;
  connect(secWidget, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
  setSecondaryWidget(secWidget);
}

RigidityFlux::~RigidityFlux()
{
  delete m_measurementTimeCalculation->histogram();
  delete m_measurementTimeCalculation;
  m_measurementTimeCalculation = 0;
  delete m_fluxCalculation;
  m_fluxCalculation = 0;
  delete m_phiFit;
  m_phiFit = 0;
}

void RigidityFlux::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  m_measurementTimeCalculation->update(event);
}

void RigidityFlux::update()
{
  if (m_particleHistogramMirrored)
    Helpers::updateMirroredHistogram(m_particleHistogramMirrored, m_particleHistogram);
  m_fluxCalculation->setMeasurementTime(m_measurementTimeCalculation->measurementTime());
  m_fluxCalculation->update();
  efficiencyCorrection();
  updateBinTitles();

  m_phiFit->fit();
  latex(m_nBinsNew)->SetTitle(qPrintable(m_phiFit->gammaLabel()));
  latex(m_nBinsNew+1)->SetTitle(qPrintable(m_phiFit->phiLabel()));
}

void RigidityFlux::finalize()
{
  update();
}

void RigidityFlux::updateBinTitles()
{
  for (int i = 0; i < m_nBinsNew; i++) {
    double bin = m_nBinsStart -1 + i;
    TLatex latext = m_fluxCalculation->binTitle(bin+1);
    double x = latext.GetX();
    double y = latext.GetY();
    QString text = latext.GetTitle();
    latex(i)->SetX(x);
    latex(i)->SetY(y);
    latex(i)->SetTitle(qPrintable(text));
  }
}

void RigidityFlux::loadEfficiencies()
{
  if (m_type == Negative) {
    m_multiLayerEff = Helpers::createMirroredHistogram(EfficiencyCorrectionSettings::instance()->histogram(EfficiencyCorrectionSettings::s_allTrackerLayerCutEfficiencyPreKey));
    m_trackFindingEff = Helpers::createMirroredHistogram(EfficiencyCorrectionSettings::instance()->histogram(EfficiencyCorrectionSettings::s_trackFindingEfficiencyPreKey));
  } else {
    m_multiLayerEff = EfficiencyCorrectionSettings::instance()->histogram(EfficiencyCorrectionSettings::s_allTrackerLayerCutEfficiencyPreKey);
    m_trackFindingEff = EfficiencyCorrectionSettings::instance()->histogram(EfficiencyCorrectionSettings::s_trackFindingEfficiencyPreKey);
  }
}

void RigidityFlux::efficiencyCorrection()
{
  m_fluxCalculation->efficiencyCorrection(m_multiLayerEff);
  m_fluxCalculation->efficiencyCorrection(m_trackFindingEff);
  m_fluxCalculation->efficiencyCorrection(0.843684 / 0.792555);
  m_fluxCalculation->efficiencyCorrection(0.999);//estimate for TOF trigger efficiency
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
  foreach(TH1D* histogram, static_cast<SimulationFluxWidget*>(secondaryWidget())->selectedHistograms()) {
    TH1D* newHisto = new TH1D(*histogram);
    if (!newHisto->GetSumw2())
      newHisto->Sumw2();
    addHistogram(newHisto, H1DPlot::HIST);
    legend()->AddEntry(newHisto, newHisto->GetTitle(), "l");
  }
  draw(Plotter::rootWidget()->GetCanvas());
  gPad->Update();
}
