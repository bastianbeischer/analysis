#include "LikelihoodRatio.hh"
#include "Likelihood.hh"
#include "Particle.hh"

#include <TAxis.h>

LikelihoodRatio::LikelihoodRatio(const Likelihood* likelihood, Enums::Particle particle, double momentum, double min, double max)
  : TF1(qPrintable(Enums::label(likelihood->type())), this, &LikelihoodRatio::rootFunctionPointer,
    min, max, 0, "LikelihoodRatio")
  , m_likelihood(likelihood)
  , m_particle(particle)
  , m_momentum(momentum)
{
  SetNpx(1000);
  SetLineColor(Particle(particle).color());
  GetXaxis()->SetTitle("p / GeV");
  GetYaxis()->SetTitle("likelihood ratio");
  GetYaxis()->SetRangeUser(0, 1);
}

LikelihoodRatio::~LikelihoodRatio()
{
}

Enums::Particle LikelihoodRatio::particle() const
{
  return m_particle;
}

double LikelihoodRatio::momentum() const
{
  return m_momentum;
}

double LikelihoodRatio::rootFunctionPointer(double* x, double*) const
{
  return m_likelihood->ratio(x[0], m_particle, m_momentum);
}
