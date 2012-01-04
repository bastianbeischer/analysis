#include "LikelihoodPDF.hh"
#include "Likelihood.hh"
#include "Particle.hh"
#include "Hypothesis.hh"

#include <TAxis.h>

LikelihoodPDF::LikelihoodPDF(const Likelihood* likelihood, const KineticVariable& variable)
  : TF1(qPrintable(Enums::label(likelihood->likelihoodVariableType()) + " " + Enums::label(variable.particle())), this
    , &LikelihoodPDF::rootFunctionPointer, likelihood->measuredValueMin(), likelihood->measuredValueMax(), 0, "LikelihoodPDF")
  , m_variable(variable)
  , m_likelihood(likelihood)
{
  SetNpx(1000);
  SetLineColor(Particle(variable.particle()).color());
}

LikelihoodPDF::~LikelihoodPDF()
{
}

void LikelihoodPDF::setVariable(const KineticVariable& variable)
{
  m_variable = variable;
}

const KineticVariable& LikelihoodPDF::variable() const
{
  return m_variable;
}

double LikelihoodPDF::integral()
{
  return Integral(m_likelihood->measuredValueMin(), m_likelihood->measuredValueMax());
}

double LikelihoodPDF::rootFunctionPointer(double* x, double*) const
{
  return m_likelihood->eval(x[0], Hypothesis(m_variable.particle(), m_variable.curvature()));
}

void LikelihoodPDF::setAxisTitle(const QString& x)
{
  GetXaxis()->SetTitle(qPrintable(x));
}
