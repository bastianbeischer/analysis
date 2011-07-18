#include "StraightLineMatrix.hh"

#include "Hit.hh"
#include <QDebug>
#include <cmath>

StraightLineMatrix::StraightLineMatrix() :
  Matrix()
{
  m_nCol = 4;
  m_solution.ResizeTo(m_nCol);
}

StraightLineMatrix::~StraightLineMatrix()
{
}

void StraightLineMatrix::fillMatrixFromHit(TMatrixD& A, unsigned int i, double k, double c, double s) const
{
  A(i,0)            = c;
  A(i,1)            = -s;
  A(i,2)            = k*c;
  A(i,3)            = -k*s;
}
