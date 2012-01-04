#include "PDFParametersVector.hh"

PDFParametersVector::PDFParametersVector(int numberOfElements, int numberOfParameters)
  : QVector<PDFParameters>(numberOfElements, PDFParameters(numberOfParameters))
  , m_min(.0)
  , m_max(.0)
{
}

void PDFParametersVector::setRange(double min, double max)
{
  m_min = min;
  m_max = max;
}

double PDFParametersVector::min() const
{
  return m_min;
}

double PDFParametersVector::max() const
{
  return m_max;
}
