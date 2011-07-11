#include "BarTimeShiftHistogram.hh"
#include "RootStyle.hh"
#include "BarShiftPlot.hh"
#include "Constants.hh"
#include "TimeShiftContainer.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TStyle.h>

#include <QDebug>
#include <QStringList>

BarTimeShiftHistogram::BarTimeShiftHistogram(const QVector<BarShiftPlot*>& plots)
  : PostAnalysisPlot()
  , H2DPlot()
{
  setTitle("bar time shift");

  TH2D* histogram = new TH2D("barShiftHistogram", ";;;#Deltat / ns", 4, 0, 4, 4, 0, 4);

  foreach (BarShiftPlot* plot, plots) {
    int xBin = 0;
    int yBin = 0;
    if (plot->title().contains("bar shift 0x8000 0x8010")) xBin = 1;
    if (plot->title().contains("bar shift 0x8004 0x8014")) xBin = 2;
    if (plot->title().contains("bar shift 0x8008 0x8018")) xBin = 3;
    if (plot->title().contains("bar shift 0x800c 0x801c")) xBin = 4;
    if (plot->title().contains("0x8020 0x8030")) yBin = 1;
    if (plot->title().contains("0x8024 0x8034")) yBin = 2;
    if (plot->title().contains("0x8028 0x8038")) yBin = 3;
    if (plot->title().contains("0x802c 0x803c")) yBin = 4;
    histogram->SetBinContent(xBin, yBin, plot->dt() - TimeShiftContainer::desiredTimeDifference(qAbs(xBin-yBin)));
  }
  histogram->GetXaxis()->SetBinLabel(1, "8000 8010");
  histogram->GetXaxis()->SetBinLabel(2, "8004 8014");
  histogram->GetXaxis()->SetBinLabel(3, "8008 8018");
  histogram->GetXaxis()->SetBinLabel(4, "800c 801c");
  histogram->GetYaxis()->SetBinLabel(1, "8020 8030");
  histogram->GetYaxis()->SetBinLabel(2, "8024 8034");
  histogram->GetYaxis()->SetBinLabel(3, "8028 8038");
  histogram->GetYaxis()->SetBinLabel(4, "802c 803c");
  histogram->GetZaxis()->SetRangeUser(-0.8, 0.8);
  histogram->SetMarkerColor(kRed);
  setPalette(RootStyle::ResiduePalette);
  setDrawOption(COLZTEXT);
  addHistogram(histogram);
}

BarTimeShiftHistogram::~BarTimeShiftHistogram()
{}
