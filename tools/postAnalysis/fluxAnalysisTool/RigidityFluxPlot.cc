#include "RigidityFluxPlot.hh"

#include "Helpers.hh"
#include "FluxCalculation.hh"
#include "SolarModulationFit.hh"
#include "SimulationFluxWidget.hh"
#include "PostAnalysisCanvas.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "Particle.hh"
#include "Enums.hh"

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
#include <TGraphAsymmErrors.h>

#include <QDebug>
#include <QMutex>

RigidityFluxPlot::RigidityFluxPlot(PostAnalysisCanvas* canvas, TH1D* particleSpectrum, double measurementTime, Enums::ChargeSign type, TH1D* particleSpectrumLow, TH1D* particleSpectrumUp)
: PostAnalysisPlot()
, H1DPlot()
, m_canvas(canvas->canvas())
, m_particleHistogram(0)
, m_particleHistogramLow(0)
, m_particleHistogramUp(0)
, m_fluxCalculation(0)
, m_type(type)
, m_sysbelt(0)
{
  QString name = canvas->name();
  name.remove(" canvas");
  
  if (name.contains("non")) {
    m_isAlbedo = false;
  } else {
    m_isAlbedo = true;
  }
  
  if (m_type == Enums::Negative)
    m_particleHistogram = Helpers::createMirroredHistogram(particleSpectrum);
  else
    m_particleHistogram = new TH1D(*particleSpectrum);

  QString newTitle = QString(canvas->histograms1D().at(0)->GetName()) + Enums::label(type);
  m_particleHistogram->SetTitle(qPrintable(newTitle));
  m_fluxCalculation = new FluxCalculation(m_particleHistogram, measurementTime);
  
  QString title = QString("flux - ") + Enums::label(type);
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
  
//  updateBinTitles();
  
  TLegend* legend = new TLegend(.80, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->AddEntry(histogram(), "Data", "p");
  addLegend(legend);
  
  if (m_type == Enums::Negative)
    m_phiFit = new SolarModulationFit(histogram(), Particle(Enums::Electron).pdgId());
  else
    m_phiFit = new SolarModulationFit(histogram(), Particle(Enums::Proton).pdgId());
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

  if (particleSpectrumLow && particleSpectrumUp) {
    if (m_type == Enums::Negative) {
      m_particleHistogramLow = Helpers::createMirroredHistogram(particleSpectrumLow);
      m_particleHistogramUp = Helpers::createMirroredHistogram(particleSpectrumUp);
    } else {
      m_particleHistogramLow = new TH1D(*particleSpectrumLow);
      m_particleHistogramUp = new TH1D(*particleSpectrumUp);
    }

    FluxCalculation* rec = new FluxCalculation(m_particleHistogram, measurementTime);
    FluxCalculation* recLow = new FluxCalculation(m_particleHistogramLow, measurementTime);
    FluxCalculation* recUp = new FluxCalculation(m_particleHistogramUp, measurementTime);
    
    m_sysbelt = Helpers::createBeltWithSystematicUncertainty(rec->fluxHistogram(),recLow->fluxHistogram(),recUp->fluxHistogram());
    m_sysbelt->SetTitle("rec. #pm 25% #sigma_{p}");
    m_sysbelt->SetLineColor(kBlue);
    m_sysbelt->SetFillColor(kBlue);
    m_sysbelt->SetFillStyle(3002);
    
    legend->AddEntry(m_sysbelt, "rec. #pm 25% #sigma_{p}", "F");
    int low = histogram()->GetXaxis()->FindBin(0.5);
    int up = histogram()->GetXaxis()->FindBin(10);
    histogram()->GetXaxis()->SetRange(low, up);
  }
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

void RigidityFluxPlot::draw(TCanvas* canvas) {
  H1DPlot::draw(canvas);
  if (m_sysbelt)
    m_sysbelt->Draw("3SAME");
}


