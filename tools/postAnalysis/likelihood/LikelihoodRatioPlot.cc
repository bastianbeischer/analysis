#include "LikelihoodRatioPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodRatio.hh"

LikelihoodRatioPlot::LikelihoodRatioPlot(Likelihood* likelihood, double momentum)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle(QString("%1 ratio %2 GeV").arg(Enums::label(likelihood->type())).arg(momentum, 3, 'f', 1, '0'));
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it)
    if (it.key() & likelihood->particles())
      addFunction(likelihood->ratio(it.key(), momentum));
}

LikelihoodRatioPlot::~LikelihoodRatioPlot()
{
}
