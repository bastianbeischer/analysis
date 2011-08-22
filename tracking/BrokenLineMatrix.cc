#include "BrokenLineMatrix.hh"

BrokenLineMatrix::BrokenLineMatrix() :
  Matrix()
{
  m_nCol = 6;
  m_solution = new double[m_nCol];
}

BrokenLineMatrix::~BrokenLineMatrix()
{
}

void BrokenLineMatrix::fillMatrixFromHit(double** A, unsigned int i, double k, double c, double s) const
{
  int offsetXindex = k > 0. ? 0 : 1;
  int slopeXindex = k > 0. ? 3 : 4;

  A[i][offsetXindex] = c;
  A[i][2]            = -s;
  A[i][slopeXindex]  = k*c;
  A[i][5]            = -k*s;
}
