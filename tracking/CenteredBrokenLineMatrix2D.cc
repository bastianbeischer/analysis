#include "CenteredBrokenLineMatrix2D.hh"

CenteredBrokenLineMatrix2D::CenteredBrokenLineMatrix2D() :
  Matrix()
{
  m_nCol = 6;
  m_solution = new double[m_nCol];
}

CenteredBrokenLineMatrix2D::~CenteredBrokenLineMatrix2D()
{
}

void CenteredBrokenLineMatrix2D::fillMatrixFromHit(double* A, double k, double c, double s) const
{
  int slopeXindex = k > 0. ? 2 : 3;
  int slopeYindex = k > 0. ? 4 : 5;

  A[0]            = c;
  A[1]            = -s;
  A[slopeXindex]  = k*c;
  A[slopeYindex]  = -k*s;
}
