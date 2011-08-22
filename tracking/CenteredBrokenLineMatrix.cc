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

void CenteredBrokenLineMatrix::fillMatrixFromHit(double** A, unsigned int i, double k, double c, double s) const
{
  int slopeXindex = k > 0. ? 2 : 3;

  A[i][0]            = c;
  A[i][1]            = -s;
  A[i][slopeXindex]  = k*c;
  A[i][4]            = -k*s;
}
