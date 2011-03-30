#include "HeightTimePlot.hh"
#include "PostAnalysisCanvas.hh"
#include "TriggerRateTimePlot.hh"
#include "HeightTimePlot.hh"

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

HeightTimePlot::HeightTimePlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle("height time plot");
  TH2D* h = canvas->histograms2D().at(0);
  int nBinsX = h->GetXaxis()->GetNbins();
  double minX = h->GetXaxis()->GetXmin();
  double maxX = h->GetXaxis()->GetXmax();
  TH1D* histogram = new TH1D("height time plot", "", nBinsX, minX, maxX);
  for (int binX = 1; binX <= nBinsX; ++binX) {
    TH1D* projection = h->ProjectionY(qPrintable(title() + "projection"), binX, binX);
    histogram->SetBinContent(binX, projection->GetMean());
    delete projection;
  }
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
  histogram->GetXaxis()->SetTitle("time");
  histogram->GetYaxis()->SetTitle("height / km");
  histogram->SetMarkerStyle(20);
  histogram->SetMarkerSize(0.4);
  addHistogram(histogram);
}

HeightTimePlot::~HeightTimePlot()
{}

void HeightTimePlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  histogram()->SetDrawOption("P");
  gPad->Modified();
  gPad->Update();
}
