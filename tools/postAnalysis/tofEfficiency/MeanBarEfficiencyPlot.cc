#include "MeanBarEfficiencyPlot.hh"
#include "BarEfficiencyPlot.hh"

#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

MeanBarEfficiencyPlot::MeanBarEfficiencyPlot(const QVector<BarEfficiencyPlot*>& plots)
  : PostAnalysisPlot()
  , H2DPlot()
{
  setHistogram(plots.at(0)->histogram());
  /*TH2* h0 = canvases[0]->histograms2D().at(0);
  int nBinsX = h0->GetXaxis()->GetNbins();
  int nBinsY = h0->GetYaxis()->GetNbins();
  double minX = h0->GetXaxis()->GetXmin();
  double maxX = h0->GetXaxis()->GetXmax();
  double minY = h0->GetYaxis()->GetXmin();
  double maxY = h0->GetYaxis()->GetXmax();
  QString title;
  title = QString("tof efficiency");
  setTitle(title);
  TH2D* h = new TH2D(qPrintable(title), "", nBinsX, minX, maxX, nBinsY, minY, maxY);
  h->GetXaxis()->SetTitle("y_{tracker} / mm");
  h->GetYaxis()->SetTitle("x_{tracker} / mm");
  h->GetZaxis()->SetTitle("efficiency");
  int n = 0;
  for (int i = 0; i < 64; ++i) {
    QString name = canvases[i]->name();
    if (name.contains("0x8034")) {
      continue;
    }
    TH2D* hi = canvases[i]->histograms2D().at(0);
    h->Add(hi, 1);
    n++;
  }
  h->Scale(1/double(n));
  setHistogram(h);
  histogram()->GetZaxis()->SetRangeUser(0.97, 1.01);*/
}

MeanBarEfficiencyPlot::~MeanBarEfficiencyPlot()
{}

double MeanBarEfficiencyPlot::sumEntries(TH2D* h)
{
  double sum = 0;
  for (int binX = 1; binX <= h->GetXaxis()->GetNbins(); ++binX) {
    for (int binY = 1; binY <= h->GetYaxis()->GetNbins(); ++binY) {
      sum+= h->GetBinContent(binX, binY);
    }
  }
  return sum;
}
