#include "TriggerRateHeightTimePlot.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TAxis.h>
#include <TPad.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TMultiGraph.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TriggerRateHeightTimePlot::TriggerRateHeightTimePlot(PostAnalysisCanvas* trigger, PostAnalysisCanvas* height)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle("trigger rate height time");
  TH1D* h0 = static_cast<TH1D*>(trigger->histograms1D().at(0)->Clone());
  TH1D* h1 = static_cast<TH1D*>(height->histograms1D().at(0)->Clone());

  m_graph = new TGraph;
  for (int bin = 1; bin <= h0->GetXaxis()->GetNbins(); ++bin) {
    double h = h1->GetBinContent(bin);
    if (!qFuzzyCompare(h, 0.))
      m_graph->SetPoint(m_graph->GetN(), h0->GetXaxis()->GetBinCenter(bin), h);
  }
  m_graph->SetMarkerColor(kBlue);
  m_graph->SetMarkerStyle(20);
  m_graph->SetMarkerSize(0.4);

  h0->GetYaxis()->SetTitle("");
  addHistogram(h0);
  
  TLegend* legend = new TLegend(0.11, 0.81, 0.28, 0.89);
  legend->AddEntry(histogram(0), "rate / Hz", "L");
  legend->AddEntry(m_graph, "height / km", "P");
  addLegend(legend);
}

TriggerRateHeightTimePlot::~TriggerRateHeightTimePlot()
{
  delete m_graph;
}

void TriggerRateHeightTimePlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  m_graph->Draw("P");
  gPad->Modified();
  gPad->Update();
}
