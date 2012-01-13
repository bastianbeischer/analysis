#include "LikelihoodPDF.hh"
#include "Likelihood.hh"
#include "ParticleProperties.hh"
#include "Hypothesis.hh"

#include <TAxis.h>

LikelihoodPDF::LikelihoodPDF(const Likelihood* likelihood, const KineticVariable& variable)
  : TF1(qPrintable(Enums::label(likelihood->likelihoodVariableType()) + " " + Enums::label(variable.particle())),
    this, &LikelihoodPDF::rootFunctionPointer, likelihood->measuredValueMin(), likelihood->measuredValueMax(),
    likelihood->numberOfParameters() + 1, "LikelihoodPDF", "rootFunctionPointer")
  , m_normalizationFactor(1.)
  , m_variable(variable)
  , m_likelihood(likelihood)
  , m_numberOfParameters(likelihood->numberOfParameters())
{
  setScaleFactor(1.);
  SetNpx(1000);
  SetLineColor(ParticleProperties(variable.particle()).color());
}

LikelihoodPDF::~LikelihoodPDF()
{
}

void LikelihoodPDF::setScaleFactor(double factor)
{
  SetParameter(m_numberOfParameters, factor);
}

void LikelihoodPDF::setParameter(int parameterNumber, double value)
{
  Q_ASSERT(0 <= parameterNumber && parameterNumber < m_numberOfParameters);
  SetParameter(parameterNumber, value);
}

void LikelihoodPDF::setLimits(int parameterNumber, double min, double max)
{
  Q_ASSERT(0 <= parameterNumber && parameterNumber < m_numberOfParameters);
  SetParLimits(parameterNumber, min, max);
}

double LikelihoodPDF::scaleFactor() const
{
  return GetParameter(m_numberOfParameters);
}

void LikelihoodPDF::setParameters(const PDFParameters& parameters)
{
  Q_ASSERT(m_numberOfParameters == parameters.size());
  m_normalizationFactor = parameters.normalizationFactor();
  for (int i = 0; i < m_numberOfParameters; ++i)
    SetParameter(i, parameters[i]);
}

void LikelihoodPDF::setNormalizationFactor(double factor)
{
  Q_ASSERT(m_normalizationFactor > 0);
  m_normalizationFactor = factor;
}

PDFParameters LikelihoodPDF::parameters() const
{
  PDFParameters parameters(m_numberOfParameters);
  parameters.setNormalizationFactor(m_normalizationFactor);
  for (int i = 0; i < m_numberOfParameters; ++i)
    parameters[i] = GetParameter(i);
  return parameters;
}

const KineticVariable& LikelihoodPDF::variable() const
{
  return m_variable;
}

double LikelihoodPDF::integral()
{
  double k = scaleFactor();
  setScaleFactor(1.);
  double result = Integral(m_likelihood->measuredValueMin(), m_likelihood->measuredValueMax());
  setScaleFactor(k);
  return result;
}

double LikelihoodPDF::rootFunctionPointer(double* x, double* p) const
{
  PDFParameters parameters(m_numberOfParameters);
  parameters.setNormalizationFactor(m_normalizationFactor);
  for (int i = 0; i < m_numberOfParameters; ++i)
    parameters[i] = p[i];
  return p[m_numberOfParameters] * m_likelihood->eval(x[0], Hypothesis(m_variable.particle(), m_variable.curvature()), parameters);
}

void LikelihoodPDF::setAxisTitle(const QString& x)
{
  GetXaxis()->SetTitle(qPrintable(x));
}
