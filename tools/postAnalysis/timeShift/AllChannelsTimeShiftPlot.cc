#include "AllChannelsTimeShiftPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "TimeShiftContainer.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>
#include <TLine.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

AllChannelsTimeShiftPlot::AllChannelsTimeShiftPlot(const QVector<PostAnalysisCanvas*>& canvases, int ch)
  : PostAnalysisPlot()
  , H2DPlot()
{
  QString title = QString("reference channel %2").arg(ch);
  setTitle(title);

  TH2D* h = canvases[0]->histograms2D().at(0);
  int nBinsX = 8 * canvases.count();
  int nBinsY = h->GetYaxis()->GetNbins();
  double yMin = h->GetYaxis()->GetXmin();
  double yMax = h->GetYaxis()->GetXmax();
  TH2D* histogram = new TH2D(qPrintable(title + "histogram"), "", nBinsX, -0.5, nBinsX - 0.5, nBinsY, yMin, yMax);
  int counter = 0;
  foreach (PostAnalysisCanvas* canvas, canvases) {
    h = canvas->histograms2D().at(0);
    h->Draw("COLZ");
    for (int binX = 1; binX <= 8; ++binX) {
      double max = 1.;
      for (int binY = 1; binY < nBinsY; ++binY) {
        if (h->GetBinContent(binX, binY) > max)
          max = h->GetBinContent(binX, binY);
      }
      for (int binY = 1; binY < nBinsY; ++binY) {
        histogram->SetBinContent(8 * counter + binX, binY, h->GetBinContent(binX, binY) / max);
        histogram->GetXaxis()->SetBinLabel(8 * counter + binX, h->GetXaxis()->GetBinLabel(binX));
      }
    }
    ++counter;
  }

  addHistogram(histogram);
}

AllChannelsTimeShiftPlot::~AllChannelsTimeShiftPlot()
{
}
