#include "TriggerRateHeightTimePlot.hh"
#include "PostAnalysisCanvas.hh"
#include "TriggerRateTimePlot.hh"
#include "HeightTimePlot.hh"

#include <TH1.h>
#include <TAxis.h>
#include <TPad.h>
#include <TLegend.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TriggerRateHeightTimePlot::TriggerRateHeightTimePlot(TriggerRateTimePlot* trtp, HeightTimePlot* htp)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle("trigger rate height time");
  TH1D* h = 0;
  
  h = static_cast<TH1D*>(trtp->histogram()->Clone());
  h->SetLineColor(kRed);
  h->GetYaxis()->SetTitle("");
  addHistogram(h);

  h = static_cast<TH1D*>(htp->histogram()->Clone());
  h->SetLineColor(kBlue);
  h->SetMarkerColor(kBlue);
  h->GetYaxis()->SetTitle("");
  addHistogram(h);

  TLegend* legend = new TLegend(0.11, 0.81, 0.28, 0.89);
  legend->AddEntry(histogram(0), "rate / Hz", "l");
  legend->AddEntry(histogram(1), "height / km", "p");
  addLegend(legend);
}

TriggerRateHeightTimePlot::~TriggerRateHeightTimePlot()
{}

void TriggerRateHeightTimePlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  histogram(1)->SetDrawOption("SAME P");
  gPad->Modified();
  gPad->Update();
}
