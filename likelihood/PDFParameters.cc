#include "PDFParameters.hh"

PDFParameters::PDFParameters()
  : QVector<double>()
  , m_normalizationFactor(1.)
{
}

PDFParameters::PDFParameters(int numberOfParameters)
  : QVector<double>(numberOfParameters, 0)
  , m_normalizationFactor(1.)
{
}

void PDFParameters::setNormalizationFactor(double factor)
{
  m_normalizationFactor = factor;
}

double PDFParameters::normalizationFactor() const
{
  return m_normalizationFactor;
}
