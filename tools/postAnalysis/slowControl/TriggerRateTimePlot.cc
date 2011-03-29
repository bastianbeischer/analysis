#include "TriggerRateTimePlot.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TriggerRateTimePlot::TriggerRateTimePlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H2DPlot()
{
  setTitle("trigger rate");
  TH2D* h1 = canvas->histograms2D().at(0);
  h1->Draw("COL Z");
  int nBinsX = h1->GetXaxis()->GetNbins();
  double minX = h1->GetXaxis()->GetXmin();
  double maxX = h1->GetXaxis()->GetXmax();
  int nBinsY = h1->GetYaxis()->GetNbins();
  double minY = h1->GetYaxis()->GetXmin();
  double maxY = h1->GetYaxis()->GetXmax();
  int rebin = 10;
  TH2D* h2 = new TH2D(qPrintable(title()), "", nBinsX / rebin, minX, maxX, nBinsY, minY, maxY);
  h2->GetXaxis()->SetTimeDisplay(1);
  h2->GetXaxis()->SetTimeFormat("%d-%H:%M");
  h2->GetXaxis()->SetTitle("time");
  h2->GetYaxis()->SetTitle("trigger rate / Hz");
  for (int binX = 1; binX < nBinsX; ++binX) {
    for (int binY = 1; binY < nBinsY; ++binY) {
      h2->SetBinContent(binX / rebin + 1, binY, h2->GetBinContent(binX, binY) + h1->GetBinContent(binX, binY));
    }
  }
  setHistogram(h2);
}

TriggerRateTimePlot::~TriggerRateTimePlot()
{}
