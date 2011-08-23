#include "StraightLineMatrix.hh"

#include "Hit.hh"
#include <QDebug>
#include <cmath>

StraightLineMatrix::StraightLineMatrix() :
  Matrix()
{
  m_nCol = 4;
  m_solution = new double[m_nCol];
}

StraightLineMatrix::~StraightLineMatrix()
{
}

void StraightLineMatrix::fillMatrixFromHit(double* A, double k, double c, double s) const
{
  A[0] = c;
  A[1] = -s;
  A[2] = k*c;
  A[3] = -k*s;
}
