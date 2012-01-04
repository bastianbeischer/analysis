#include "ResiduePlot.hh"
#include "TimeResolutionAnalysis.hh"
#include "TimeOfFlightHistogram.hh"
#include "Constants.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TAxis.h>

#include <QDebug>

ResiduePlot::ResiduePlot(const QVector<TimeOfFlightHistogram*>& histograms, const TimeResolutionAnalysis* const analysis)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle("time resolution residue plot");

  int nBins = 0;
  foreach (TimeOfFlightHistogram* h, histograms)
    if (h->fitGood())
      ++nBins;

  TH1D* histogram = new TH1D("residueHistogram", "", nBins, 0, nBins);
  addHistogram(histogram);
  setDrawOption(P);

  int counter = 0;
  foreach (TimeOfFlightHistogram* h, histograms) {
    if (!h->fitGood())
      continue;
    double res = h->sigma() - Helpers::addQuad(analysis->sigmaIK(h->i(), h->k()), analysis->sigmaIK(h->j(), h->l()));
    histogram->SetBinContent(counter, res);
    double resError = Helpers::addQuad(h->sigmaError(), 0 /*TODO: error from analysis*/);
    histogram->SetBinError(counter, resError);
    ++counter;
  }

  yAxis()->SetRangeUser(-1.0, 1.0);
  setAxisTitle("", "residue / ns");
}

ResiduePlot::~ResiduePlot()
{}
