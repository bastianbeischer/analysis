#include "TRDLikelihoodFunctionsPlot.hh"

#include "TRDLikelihoods.hh"
#include "TRDReconstruction.hh"

#include <TH1D.h>
#include <TF1.h>

TRDLikelihoodFunctionsPlot::TRDLikelihoodFunctionsPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTRD)
  , H1DPlot()
{
  setTitle("TRD pdfs");
  setAxisTitle(TRDReconstruction::xAxisTitle(), "");
  addHistogram(new TH1D(qPrintable(title()),"", 100, 0, TRDReconstruction::spectrumUpperLimit()));

  TRDLikelihoods* lhs = TRDLikelihoods::instance();
  addFunction(new TF1(*lhs->getLHFunctionNonTR()));
  addFunction(new TF1(*lhs->getLHFunctionTR()));
  function(1)->SetLineColor(kRed);
  for (int i = 0; i < 8; ++i) {
    addFunction(new TF1(*lhs->getLHFunctionTRLayer(i)));
    function(i+2)->SetLineWidth(0.1);
    function(i+2)->SetLineColor(kMagenta-4+(i%2)*4+i);
  }
}

TRDLikelihoodFunctionsPlot::~TRDLikelihoodFunctionsPlot()
{
}

void TRDLikelihoodFunctionsPlot::processEvent(const QVector<Hit*>&, Particle*, SimpleEvent*)
{
}
