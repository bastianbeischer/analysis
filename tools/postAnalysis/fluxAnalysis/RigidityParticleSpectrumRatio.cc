#include "RigidityParticleSpectrumRatio.hh"

#include "SimulationFluxRatioWidget.hh"
#include "BessFluxRatioWidget.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1D.h>
#include <TLegendEntry.h>
#include <TCanvas.h>
#include <TList.h>
#include <TH1D.h>
#include <TPad.h>
#include <TLegend.h>

#include <QString>
#include <QWidget>
#include <QVBoxLayout>

RigidityParticleSpectrumRatio::RigidityParticleSpectrumRatio(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_canvas(canvas->canvas())
  , m_simulationWidget(new SimulationFluxRatioWidget)
  , m_bessWidget(new BessFluxRatioWidget)
{
  QString name = canvas->name();
  name.replace("canvas", "histogram");
  TH1D* histogram = new TH1D(*canvas->histograms1D().at(0));
  setTitle(name);
  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());

  TLegend* leg = new TLegend(.80, .72, .98, .98);
  leg->SetFillColor(kWhite);
  addLegend(leg);
  legend()->AddEntry(histogram, "Data", "p");

  QWidget* secWidget = new QWidget();
  QVBoxLayout* layout = new QVBoxLayout(secWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_simulationWidget);
  connect(m_simulationWidget, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
  layout->addWidget(m_bessWidget);
  connect(m_bessWidget, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
  setSecondaryWidget(secWidget);
}

RigidityParticleSpectrumRatio::~RigidityParticleSpectrumRatio()
{
}

void RigidityParticleSpectrumRatio::selectionChanged()
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
    newHisto->SetTitle("MC (smoothed)");
    newHisto->Smooth(1);
    addHistogram(newHisto, H1DPlot::HIST);
    legend()->AddEntry(newHisto, newHisto->GetTitle(), "l");
  }
  foreach(TH1D* histogram, m_bessWidget->selectedHistograms()) {
    TH1D* newHisto = new TH1D(*histogram);
    if (!newHisto->GetSumw2())
      newHisto->Sumw2();
    addHistogram(newHisto, H1DPlot::P);
    legend()->AddEntry(newHisto, newHisto->GetTitle(), "p");
  }
  draw(m_canvas);
  gPad->Update();
}

void RigidityParticleSpectrumRatio::draw(TCanvas* canvas) {
  H1DPlot::draw(canvas);
  if (histogram())
    histogram()->Draw("3SAME");
}