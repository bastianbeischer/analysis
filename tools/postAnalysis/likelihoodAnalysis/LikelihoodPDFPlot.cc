#include "LikelihoodPDFPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"

#include <QDebug>

LikelihoodPDFPlot::LikelihoodPDFPlot(Likelihood* likelihood, double momentum)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle(QString("%1 PDF %2 GeV").arg(likelihood->title()).arg(momentum, 3, 'f', 1, '0'));
  Enums::Particles particles = likelihood->particles();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it) {
    if (!(it.key() & particles))
      continue;
    LikelihoodPDF* pdf = likelihood->pdf(KineticVariable(it.key(), Enums::Momentum, momentum));
    if (pdf)
      addFunction(pdf);
  }
}

LikelihoodPDFPlot::~LikelihoodPDFPlot()
{
}
