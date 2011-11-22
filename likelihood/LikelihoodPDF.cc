#include "LikelihoodPDF.hh"
#include "Likelihood.hh"
#include "Particle.hh"
#include "Hypothesis.hh"

#include <TAxis.h>

LikelihoodPDF::LikelihoodPDF(const Likelihood* likelihood, const KineticVariable& variable)
  : TF1(qPrintable(Enums::label(likelihood->likelihoodVariableType())), this, &LikelihoodPDF::rootFunctionPointer,
    likelihood->min(), likelihood->max(), 0, "LikelihoodPDF")
  , m_variable(variable)
  , m_likelihood(likelihood)
{
  SetNpx(1000);
  SetLineColor(Particle(variable.particle()).color());
  GetXaxis()->SetTitle(qPrintable(Enums::label(likelihood->measuredValueType())));
  GetYaxis()->SetTitle("probability");
}

LikelihoodPDF::~LikelihoodPDF()
{
}

const KineticVariable& LikelihoodPDF::variable() const
{
  return m_variable;
}

double LikelihoodPDF::integral()
{
  return Integral(m_likelihood->min(), m_likelihood->max());
}

double LikelihoodPDF::rootFunctionPointer(double* x, double*) const
{
  return m_likelihood->eval(x[0], Hypothesis(m_variable.particle(), m_variable.curvature()));
}
