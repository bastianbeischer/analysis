#include "RigidityFluxPlot.hh"

#include "Helpers.hh"
#include "FluxCalculation.hh"
#include "SolarModulationFit.hh"
#include "SimulationFluxWidget.hh"
#include "PostAnalysisCanvas.hh"
#include "EfficiencyCorrectionSettings.hh"

#include <TH1D.h>
#include <TH2D.h>
#include <TAxis.h>
#include <TPad.h>
#include <TF1.h>
#include <TList.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLegendEntry.h>

#include <QDebug>
#include <QMutex>

RigidityFluxPlot::RigidityFluxPlot(PostAnalysisCanvas* canvas, TH1D* particleSpectrum, double measurementTime, Type type)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_canvas(canvas->canvas())
  , m_particleHistogram(0)
  , m_fluxCalculation(0)
  , m_type(type)
{
  m_typeNames.insert(Positive, "positive");
  m_typeNames.insert(Negative, "negative");
//  loadEfficiencies();

  QString name = canvas->name();
  name.remove(" canvas");

  if (name.contains("non")) {
    m_isAlbedo = false;
  } else {
    m_isAlbedo = true;
  }

  if (m_type == Negative)
    m_particleHistogram = Helpers::createMirroredHistogram(particleSpectrum);
  else
    m_particleHistogram = new TH1D(*particleSpectrum);

  QString newTitle = QString(canvas->histograms1D().at(0)->GetName()) + m_typeNames[type];
  m_particleHistogram->SetTitle(qPrintable(newTitle));

  //corrections befor flux calculation
//  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(m_particleHistogram, m_multiLayerEff);
//  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(m_particleHistogram, m_trackFindingEff);
//  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(m_particleHistogram, 0.843684 / 0.792555);
//  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(m_particleHistogram, 0.999);//estimate for TOF trigger efficiency
  //....todo and particle counting from original histogram
  m_fluxCalculation = new FluxCalculation(m_particleHistogram, measurementTime);

  QString title = QString("flux - ") + m_typeNames[m_type];
  if (m_isAlbedo)
    title.append(" albedo");

  setTitle(title);
  addHistogram(m_fluxCalculation->fluxHistogram());
  histogram()->GetXaxis()->SetMoreLogLabels(true);
  setAxisTitle(m_fluxCalculation->fluxHistogram()->GetXaxis()->GetTitle(), m_fluxCalculation->fluxHistogram()->GetYaxis()->GetTitle());

  const int nBins = histogram()->GetNbinsX();
  if (nBins%2 == 0)
    m_nBinsNew = nBins / 2;
  else
    m_nBinsNew = (nBins - 1) / 2;

  if (nBins%2 == 0)
    m_nBinsStart = m_nBinsNew + 1;
  else
    m_nBinsStart = m_nBinsNew + 2;
  histogram()->GetXaxis()->SetRange(m_nBinsStart, nBins);

  for(int i = 0; i < m_nBinsNew; i++) {
    TLatex* latex = new TLatex;
    latex->SetTextFont(42);
    latex->SetTextSize(0.02);
    latex->SetLineWidth(2);
    latex->SetTextAngle(45);
    latex->SetTextColor(kBlack);
    addLatex(latex);
  }

  updateBinTitles();

  TLegend* legend = new TLegend(.80, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->AddEntry(histogram(), "Data", "p");
  addLegend(legend);

  m_phiFit = new SolarModulationFit(histogram());
  addFunction(m_phiFit->fit());
  TLatex* gammaLatex = RootPlot::newLatex(.4, .88);
  TLatex* phiLatex = RootPlot::newLatex(.4, .83);
  addLatex(gammaLatex);
  addLatex(phiLatex);

  m_phiFit->fit();
  latex(m_nBinsNew)->SetTitle(qPrintable(m_phiFit->gammaLabel()));
  latex(m_nBinsNew+1)->SetTitle(qPrintable(m_phiFit->phiLabel()));

  SimulationFluxWidget* secWidget = new SimulationFluxWidget;
  connect(secWidget, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
  setSecondaryWidget(secWidget);
}

RigidityFluxPlot::~RigidityFluxPlot()
{
  if (m_particleHistogram) {
    delete m_particleHistogram;
    m_particleHistogram = 0;
  }
  if (m_fluxCalculation) {
    delete m_fluxCalculation;
    m_fluxCalculation = 0;
  }
}

void RigidityFluxPlot::updateBinTitles()
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

//void RigidityFluxPlot::loadEfficiencies()
//{
//  const EfficiencyCorrectionSettings::BinQuantity quantity = EfficiencyCorrectionSettings::Unfolded;
//  if (m_type == Negative) {
//    m_multiLayerEff = Helpers::createMirroredHistogram(EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency(quantity));
//    m_trackFindingEff = Helpers::createMirroredHistogram(EfficiencyCorrectionSettings::instance()->trackFindingEfficiency(quantity));
//  } else {
//    m_multiLayerEff = EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency(quantity);
//    m_trackFindingEff = EfficiencyCorrectionSettings::instance()->trackFindingEfficiency(quantity);
//  }
//}

void RigidityFluxPlot::selectionChanged()
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
  draw(m_canvas);
  gPad->Update();
}
