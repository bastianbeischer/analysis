#include "MeanBarEfficiencyPlot.hh"
#include "BarEfficiencyPlot.hh"

#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TLatex.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

MeanBarEfficiencyPlot::MeanBarEfficiencyPlot(const QVector<BarEfficiencyPlot*>& plots)
  : PostAnalysisPlot()
  , H2DPlot()
{
  TH2* h0 = plots.at(0)->histogram();
  int nBinsX = h0->GetXaxis()->GetNbins();
  int nBinsY = h0->GetYaxis()->GetNbins();
  double minX = h0->GetXaxis()->GetXmin();
  double maxX = h0->GetXaxis()->GetXmax();
  double minY = h0->GetYaxis()->GetXmin();
  double maxY = h0->GetYaxis()->GetXmax();
  setTitle("mean tof efficiency");
  TH2D* h = new TH2D(qPrintable(title()), "", nBinsX, minX, maxX, nBinsY, minY, maxY); 
  setAxisTitle("y_{tracker} / mm", "x_{tracker} / mm", "efficiency");
  int n = 0;
  for (int i = 0; i < plots.size(); ++i) {
    TH2D* hi = plots.at(i)->histogram();
    h->Add(hi, 1);
    ++n;
  }
  h->Scale(1./n);

  double sum = sumEntries(h);
  double mean = sum / (h->GetXaxis()->GetNbins() * h->GetYaxis()->GetNbins());

  addHistogram(h);

  TLatex* latex = 0;
  latex = RootPlot::newLatex(.3, .82);
  latex->SetTitle(qPrintable(QString("mean efficiency = %1").arg(mean)));
  addLatex(latex);
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
