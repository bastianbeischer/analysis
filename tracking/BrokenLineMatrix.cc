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

void BrokenLineMatrix::fillMatrixFromHit(double* A, double k, double c, double s) const
{
  int offsetXindex = k > 0. ? 0 : 1;
  int slopeXindex = k > 0. ? 3 : 4;

  A[offsetXindex] = c;
  A[2]            = -s;
  A[slopeXindex]  = k*c;
  A[5]            = -k*s;
}
