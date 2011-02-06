#include "TimeDifferencePlot.hh"
#include "TimeDifferenceFunction.hh"

#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TimeDifferencePlot::TimeDifferencePlot(TCanvas* canvas, TimeDifferenceFunction* f)
  : PostAnalysisPlot()
  , H2DPlot()
{
  TH2* histogram = 0;
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      histogram = static_cast<TH2*>(canvas->GetListOfPrimitives()->At(i));
  }
  QString title;
  title = QString("%1 residual").arg(canvas->GetName());
  setTitle(title);
  int nBinsX = histogram->GetXaxis()->GetNbins();
  int nBinsY = histogram->GetYaxis()->GetNbins();
  double minX = histogram->GetXaxis()->GetXmin();
  double maxX = histogram->GetXaxis()->GetXmax();
  double minY = histogram->GetYaxis()->GetXmin();
  double maxY = histogram->GetYaxis()->GetXmax();
  title = QString("%1 residual histogram").arg(canvas->GetName());
  setTitle(title);
  TH2D* h = new TH2D(qPrintable(title), "", nBinsX, minX, maxX, nBinsY, minY, maxY);
  for (int binX = 1; binX <= nBinsX; ++binX) {
    for (int binY = 1; binY <= nBinsY; ++binY) {
      h->SetBinContent(binX, binY, histogram->GetBinContent(binX, binY));
    }
  }
  h->Add(f->histogram(), -1);
  setHistogram(h);
}

TimeDifferencePlot::~TimeDifferencePlot()
{}
