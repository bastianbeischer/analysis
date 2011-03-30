#include "TriggerRateHeightCorrelationPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "TriggerRateTimePlot.hh"
#include "HeightTimePlot.hh"

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

TriggerRateHeightCorrelationPlot::TriggerRateHeightCorrelationPlot(TriggerRateTimePlot* trtp, HeightTimePlot* htp)
  : PostAnalysisPlot()
  , GraphPlot()
{
  setTitle("trigger rate height correlation");
  int nBinsX = htp->histogram()->GetXaxis()->GetNbins();
  TGraph* g = new TGraph;
  for (int bin = 1; bin <= nBinsX; ++bin) {
    g->SetPoint(bin - 1, htp->histogram()->GetBinContent(bin), trtp->histogram()->GetBinContent(bin));
  }
  g->SetMarkerStyle(20);
  g->SetMarkerSize(0.4);
  addGraph(g, "P");
  m_multiGraph->SetTitle(";height / km;trigger rate / Hz");
}

TriggerRateHeightCorrelationPlot::~TriggerRateHeightCorrelationPlot()
{}
