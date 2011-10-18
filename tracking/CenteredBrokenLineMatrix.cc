#include "CenteredBrokenLineMatrix.hh"

CenteredBrokenLineMatrix::CenteredBrokenLineMatrix() :
  Matrix()
{
  m_nCol = 5;
  m_solution = new double[m_nCol];
}

CenteredBrokenLineMatrix::~CenteredBrokenLineMatrix()
{
}

void CenteredBrokenLineMatrix::fillMatrixFromHit(double* A, double k, double c, double s) const
{
  int slopeXindex = k > 0. ? 2 : 3;

  A[0]            = c;
  A[1]            = -s;
  A[slopeXindex]  = k*c;
  A[4]            = -k*s;
}
