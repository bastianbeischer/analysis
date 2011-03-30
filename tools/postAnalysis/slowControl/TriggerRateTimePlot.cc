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
  , H1DPlot()
{
  setTitle("trigger rate");
  TH1D* h1 = canvas->histograms1D().at(0);
  int nBins = h1->GetXaxis()->GetNbins();
  double min = h1->GetXaxis()->GetXmin();
  double max = h1->GetXaxis()->GetXmax();
  TH1D* h2 = new TH1D(qPrintable(title()), "", nBins, min, max);
  h2->GetXaxis()->SetTimeDisplay(1);
  h2->GetXaxis()->SetTimeFormat("%d-%H:%M");
  h2->GetXaxis()->SetTitle("time");
  h2->GetYaxis()->SetTitle("trigger rate / Hz");
  for (int bin = 1; bin < nBins; ++bin) {
    h2->SetBinContent(bin, h1->GetBinContent(bin));
  }
  addHistogram(h2);
}

TriggerRateTimePlot::~TriggerRateTimePlot()
{}
