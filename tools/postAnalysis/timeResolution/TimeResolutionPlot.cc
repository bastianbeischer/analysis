#include "TimeResolutionPlot.hh"
#include "TimeResolutionAnalysis.hh"
#include "PostAnalysisCanvas.hh"
#include "Constants.hh"

#include <TH1D.h>
#include <TAxis.h>
#include <TLegend.h>

#include <QDebug>

TimeResolutionPlot::TimeResolutionPlot(const TimeResolutionAnalysis* const analysis, Type type, PostAnalysisCanvas* simulationCanvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
  QString title = "time resolution plot";
  QString postFix = (type == Variance) ? QString(" variance") : QString(" standard deviation");
  bool mc = simulationCanvas;
  if (mc)
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

  TLegend* legend = new TLegend(.65, .65, .88, .88);
  addLegend(legend);

  for (int i = 0; i < 4; ++i) {
    TH1D* histogram = new TH1D(qPrintable(QString("reconstructed i = %1").arg(i) + postFix), "", nBins,
      -Constants::tofBarLength / 2., Constants::tofBarLength / 2.);
    histogram->SetLineStyle(2);
    histogram->SetLineColor(i == 2 ? kMagenta : i+1);
    histogram->SetMarkerColor(i == 2 ? kMagenta : i+1);
    histogram->SetMarkerStyle(22);
    for (int k = 0; k < nBins; ++k)
      histogram->SetBinContent(k+1, (type == Variance) ? analysis->vIK(i, k, r) : analysis->sigmaIK(i, k, r));
    addHistogram(histogram, mc ? P : LP);
    legend->AddEntry(histogram, qPrintable(QString("upper double bar %1").arg(i + 1)));
  }

  for (int j = 0; j < 4; ++j) {
    TH1D* histogram = new TH1D(qPrintable(QString("reconstructed j = %1").arg(j) + postFix), "", nBins,
      -Constants::tofBarLength / 2., Constants::tofBarLength / 2.);
    histogram->SetLineStyle(2);
    histogram->SetLineColor(j == 2 ? kMagenta : j+1);
    histogram->SetMarkerColor(j == 2 ? kMagenta : j+1);
    histogram->SetMarkerStyle(23);
    for (int l = 0; l < nBins; ++l)
      histogram->SetBinContent(l+1, (type == Variance) ? analysis->vJL(j, l, r) : analysis->sigmaJL(j, l, r));
    histogram->SetLineStyle(2);
    addHistogram(histogram, mc ? P : LP);
    legend->AddEntry(histogram, qPrintable(QString("lower double bar %1").arg(j + 1)));
  }

  setDrawOption(mc ? P : LP);
  yAxis()->SetRangeUser(0., 1.);
  setAxisTitle("y / mm", (type == Variance) ? QString("variance / ns^{2}") : QString("#sigma / ns"));
  if (mc) {
    foreach (TH1D* histogram, simulationCanvas->histograms1D())
      addHistogram(static_cast<TH1D*>(histogram->Clone()));
  }
}

TimeResolutionPlot::~TimeResolutionPlot()
{}
