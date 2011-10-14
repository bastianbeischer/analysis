#include "LikelihoodPDFPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"

LikelihoodPDFPlot::LikelihoodPDFPlot(Likelihood* likelihood, double momentum)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle(QString("%1 PDF %2 GeV").arg(Enums::label(likelihood->type())).arg(momentum, 3, 'f', 1, '0'));
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it)
    if (it.key() & likelihood->particles())
      addFunction(likelihood->pdf(it.key(), momentum));
}

LikelihoodPDFPlot::~LikelihoodPDFPlot()
{
}
