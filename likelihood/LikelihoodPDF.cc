#include "LikelihoodPDF.hh"
#include "Likelihood.hh"
#include "Particle.hh"

#include <TAxis.h>

LikelihoodPDF::LikelihoodPDF(const Likelihood* likelihood, Enums::Particle particle, double momentum)
  : TF1(qPrintable(Enums::label(likelihood->type())), this, &LikelihoodPDF::rootFunctionPointer,
    likelihood->min(), likelihood->max(), 0, "LikelihoodPDF")
  , m_likelihood(likelihood)
  , m_particle(particle)
  , m_momentum(momentum)
{
  SetNpx(1000);
  SetLineColor(Particle(particle).color());
  GetXaxis()->SetTitle("p / GeV");
  GetYaxis()->SetTitle("probability");
}

LikelihoodPDF::~LikelihoodPDF()
{
}

Enums::Particle LikelihoodPDF::particle() const
{
  return m_particle;
}

double LikelihoodPDF::momentum() const
{
  return m_momentum;
}


double LikelihoodPDF::rootFunctionPointer(double* x, double*) const
{
  return m_likelihood->eval(x[0], m_particle, m_momentum);
}
