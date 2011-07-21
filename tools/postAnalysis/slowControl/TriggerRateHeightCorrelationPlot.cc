#include "TriggerRateHeightCorrelationPlot.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TMultiGraph.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TriggerRateHeightCorrelationPlot::TriggerRateHeightCorrelationPlot(PostAnalysisCanvas* trigger, PostAnalysisCanvas* height)
  : PostAnalysisPlot()
  , GraphPlot()
{
  setTitle("trigger rate height correlation");
  TH1D* h0 = trigger->histograms1D().at(0);
  TH1D* h1 = height->histograms1D().at(0);
  int nBins = h0->GetXaxis()->GetNbins();
  TGraph* g = new TGraph;
  for (int bin = 1; bin <= nBins; ++bin)
    g->SetPoint(g->GetN(), h1->GetBinContent(bin), h0->GetBinContent(bin));
  g->SetMarkerStyle(20);
  g->SetMarkerSize(0.4);
  addGraph(g, P);
  setAxisTitle("height / km", "trigger rate / Hz");
}

TriggerRateHeightCorrelationPlot::~TriggerRateHeightCorrelationPlot()
{}
