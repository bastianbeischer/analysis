#include "PullPlot.hh"
#include "TimeResolutionAnalysis.hh"
#include "TimeOfFlightHistogram.hh"
#include "Constants.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TAxis.h>

#include <QDebug>

PullPlot::PullPlot(const QVector<TimeOfFlightHistogram*>& histograms, const TimeResolutionAnalysis* const analysis)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle("time resolution pull plot");

  TH1D* histogram = new TH1D("pullHistogram", "", 400, -10.0, 10.0);
  addHistogram(histogram);

  int counter = 0;
  foreach (TimeOfFlightHistogram* h, histograms) {
    if (!h->fitGood())
      continue;
    double res = h->sigma() - Helpers::addQuad(analysis->sigmaIK(h->i(), h->k()), analysis->sigmaIK(h->j(), h->l()));
    double resError = Helpers::addQuad(h->sigmaError(), 0 /*TODO: error from analysis*/);
    histogram->Fill(res / resError);
    ++counter;
  }
  setAxisTitle("pull", "");
}

PullPlot::~PullPlot()
{}
