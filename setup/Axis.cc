#include "Axis.hh"

#include <cmath>

Axis::Axis(double x0, double x1, int nBins) :
  m_x0(x0),
  m_x1(x1),
  m_nBins(nBins)
{
}

Axis::~Axis()
{
}

double Axis::binCenter(int binX)
{
  return m_x0 + (binX + 0.5) * (m_x1-m_x0)/((double)m_nBins);
}

int Axis::bin(double x)
{
  int value = (int) floor((x-m_x0)/(m_x1-m_x0) * m_nBins);
  if (value < 0) return -1;
  if (value > m_nBins) return m_nBins;
  return value;
}
