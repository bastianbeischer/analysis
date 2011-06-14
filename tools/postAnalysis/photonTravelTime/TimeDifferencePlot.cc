#include "TimeDifferencePlot.hh"
#include "TimeDifferenceFunction.hh"
#include "PostAnalysisCanvas.hh"

#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TimeDifferencePlot::TimeDifferencePlot(PostAnalysisCanvas* canvas, TimeDifferenceFunction* f)
  : PostAnalysisPlot()
  , H2DPlot()
{
  TH2D* histogram = canvas->histograms2D().at(0);
  QString title;
  title = QString("%1 residual").arg(canvas->name());
  setTitle(title);
  int nBinsX = histogram->GetXaxis()->GetNbins();
  int nBinsY = histogram->GetYaxis()->GetNbins();
  double minX = histogram->GetXaxis()->GetXmin();
  double maxX = histogram->GetXaxis()->GetXmax();
  double minY = histogram->GetYaxis()->GetXmin();
  double maxY = histogram->GetYaxis()->GetXmax();
  title = QString("%1 residual histogram").arg(canvas->name());
  setTitle(title);
  TH2D* h = new TH2D(qPrintable(title), "", nBinsX, minX, maxX, nBinsY, minY, maxY);
  for (int binX = 1; binX <= nBinsX; ++binX) {
    for (int binY = 1; binY <= nBinsY; ++binY) {
      h->SetBinContent(binX, binY, histogram->GetBinContent(binX, binY));
    }
  }
  h->Add(f->histogram(), -1);
  h->SetMarkerColor(kRed);
  setPalette(RootStyle::ResiduePalette);
  setDrawOption(COLZTEXT);
  addHistogram(h);
}

TimeDifferencePlot::~TimeDifferencePlot()
{}

void TimeDifferencePlot::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  histogram()->GetZaxis()->SetRangeUser(-1, 1);
}
