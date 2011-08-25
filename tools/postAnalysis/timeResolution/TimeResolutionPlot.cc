#include "TimeResolutionPlot.hh"
#include "TimeResolutionAnalysis.hh"
#include "PostAnalysisCanvas.hh"
#include "Constants.hh"

#include <TH1D.h>
#include <TAxis.h>

#include <QDebug>

TimeResolutionPlot::TimeResolutionPlot(const TimeResolutionAnalysis* const analysis, Type type, PostAnalysisCanvas* simulationCanvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
  QString title = "time resolution plot";
  QString postFix = (type == Variance) ? QString(" variance") : QString(" standard deviation");
  if (simulationCanvas)
    postFix.append(" with MC");
  title.append(postFix);
  setTitle(title);
  
  int nBins = analysis->nBins();

  double r = -1;
  if (simulationCanvas) {
    foreach (TH1D* histogram, simulationCanvas->histograms1D()) {
      if (!strcmp(histogram->GetName(), "resolutionHistogram_3_1")) {
        double sigma = histogram->GetBinContent(histogram->GetXaxis()->GetNbins());
        r = sigma * sigma;
      }
    }
  }

  for (int i = 0; i < 4; ++i) {
    TH1D* histogram = new TH1D(qPrintable(QString("reconstructed i = %1").arg(i) + postFix), "", nBins,
      -Constants::tofBarLength / 2., Constants::tofBarLength / 2.);
    histogram->SetLineStyle(1);
    histogram->SetLineColor(1+i);
    histogram->SetMarkerColor(1+i);
    histogram->SetMarkerStyle(22);
    for (int k = 0; k < nBins; ++k)
      histogram->SetBinContent(k+1, (type == Variance) ? analysis->vIK(i, k, r) : analysis->sigmaIK(i, k, r));
    addHistogram(histogram, P);
  }

  for (int j = 0; j < 4; ++j) {
    TH1D* histogram = new TH1D(qPrintable(QString("reconstructed j = %1").arg(j) + postFix), "", nBins,
      -Constants::tofBarLength / 2., Constants::tofBarLength / 2.);
    histogram->SetLineStyle(2);
    histogram->SetLineColor(1+j);
    histogram->SetMarkerColor(1+j);
    histogram->SetMarkerStyle(23);
    for (int l = 0; l < nBins; ++l)
      histogram->SetBinContent(l+1, (type == Variance) ? analysis->vJL(j, l, r) : analysis->sigmaJL(j, l, r));
    addHistogram(histogram, P);
  }

  setDrawOption(P);
  yAxis()->SetRangeUser(0., 1.);
  setAxisTitle("y / mm", (type == Variance) ? QString("variance / ns^{2}") : QString("#sigma / ns"));
  if (simulationCanvas) {
    foreach (TH1D* histogram, simulationCanvas->histograms1D())
      addHistogram(static_cast<TH1D*>(histogram->Clone()));
  }
}

TimeResolutionPlot::~TimeResolutionPlot()
{}
