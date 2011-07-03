#include "TRDLikelihoodFunctionsPlot.hh"

#include "TRDLikelihoods.hh"
#include "TRDCalculations.hh"

#include <TH1D.h>
#include <TF1.h>

TRDLikelihoodFunctionsPlot::TRDLikelihoodFunctionsPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTRD)
  , H1DPlot()
{
  setTitle("TRD pdfs");
  setAxisTitle(TRDCalculations::xAxisTitle(), "");
  addHistogram(new TH1D(qPrintable(title()),"", 100, 0, TRDCalculations::spectrumUpperLimit()));

  TRDLikelihoods* lhs = TRDLikelihoods::instance();
  addFunction(new TF1(*lhs->getLHFunctionNonTR()));
}

TRDLikelihoodFunctionsPlot::~TRDLikelihoodFunctionsPlot()
{
}

void TRDLikelihoodFunctionsPlot::processEvent(const QVector<Hit*>&, Particle*, SimpleEvent*)
{
}
