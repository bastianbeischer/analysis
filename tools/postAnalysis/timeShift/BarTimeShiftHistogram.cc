#include "BarTimeShiftHistogram.hh"

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

BarTimeShiftHistogram::BarTimeShiftHistogram(const QVector<TCanvas*>& canvases)
  : PostAnalysisPlot()
  , H2DPlot()
{
  setTitle("bar time shift");
  QVector<TH1D*> histograms;
  foreach(TCanvas* canvas, canvases) {
    for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
      if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH1D"))
        if (!strstr(canvas->GetListOfPrimitives()->At(i)->GetName(), "all")) {
          histograms.append(static_cast<TH1D*>(canvas->GetListOfPrimitives()->At(i)));
        }
    }
  }

  TH2D* histogram = new TH2D("barShiftHistogram", ";id;id;#Deltat / ns", 4, 0, 4, 4, 0, 4);
  foreach(TH1D* h, histograms) {
    int xBin = 0;
    int yBin = 0;
    if (strstr(h->GetName(), "time resolution 0x8000 0x8010")) xBin = 1;
    if (strstr(h->GetName(), "time resolution 0x8004 0x8014")) xBin = 2;
    if (strstr(h->GetName(), "time resolution 0x8008 0x8018")) xBin = 3;
    if (strstr(h->GetName(), "time resolution 0x800c 0x801c")) xBin = 4;
    if (strstr(h->GetName(), "0x8020 0x8030 histogram")) yBin = 1;
    if (strstr(h->GetName(), "0x8024 0x8034 histogram")) yBin = 2;
    if (strstr(h->GetName(), "0x8028 0x8038 histogram")) yBin = 3;
    if (strstr(h->GetName(), "0x802c 0x803c histogram")) yBin = 4;
    histogram->SetBinContent(xBin, yBin, h->GetMean() - 2.6666);
  }
  histogram->GetXaxis()->SetBinLabel(1, "8000 8010");
  histogram->GetXaxis()->SetBinLabel(2, "8004 8014");
  histogram->GetXaxis()->SetBinLabel(3, "8008 8018");
  histogram->GetXaxis()->SetBinLabel(4, "800c 801c");
  histogram->GetYaxis()->SetBinLabel(1, "8020 8030");
  histogram->GetYaxis()->SetBinLabel(2, "8024 8034");
  histogram->GetYaxis()->SetBinLabel(3, "8028 8038");
  histogram->GetYaxis()->SetBinLabel(4, "802c 803c");
  histogram->GetZaxis()->SetRangeUser(-0.5, 0.5);
  //histogram->Draw("colz");
  setHistogram(histogram);
}

BarTimeShiftHistogram::~BarTimeShiftHistogram()
{}
