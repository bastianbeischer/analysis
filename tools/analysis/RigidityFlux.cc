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
  m_fluxCalculation->update(m_measurementTimeCalculation->measurementTime());
  efficiencyCorrection();

  updateBinTitles();

  m_phiFit->fit();
  latex(m_nBinsNew)->SetTitle(qPrintable(m_phiFit->gammaLabel()));
  latex(m_nBinsNew + 1)->SetTitle(qPrintable(m_phiFit->phiLabel()));
}

void RigidityFlux::finalize()
{
  update();
}

void RigidityFlux::updateBinTitles()
{
  for (int i = 0; i < m_nBinsNew; i++) {
    double bin = m_nBinsStart + i - 1;
    double x = xAxis()->GetBinCenterLog(bin + 1);
    double y = histogram()->GetBinContent(bin + 1);
    double value = m_particleHistogram->GetBinContent(bin);
    QString text = "#scale[0.6]{"+QString::number(value,'d',0)+"}";
    latex(i)->SetX(x);
    latex(i)->SetY(y);
    latex(i)->SetTitle(qPrintable(text));
  }
}

void RigidityFlux::loadEfficiencies()
{
  if (m_type == Negative) {
    m_multiLayerEff = Helpers::createMirroredHistogram(EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency());
    m_trackFindingEff = Helpers::createMirroredHistogram(EfficiencyCorrectionSettings::instance()->trackFindingEfficiency());
  } else {
    m_multiLayerEff = EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency();
    m_trackFindingEff = EfficiencyCorrectionSettings::instance()->trackFindingEfficiency();
  }
}

void RigidityFlux::efficiencyCorrection()
{
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(histogram(), m_multiLayerEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(histogram(), m_trackFindingEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(histogram(), 0.843684 / 0.792555);
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
