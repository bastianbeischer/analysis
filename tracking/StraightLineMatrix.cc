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

// bool StraightLineMatrix::checkInvertability(const QVector<Hit*>& hits) const
// {
//   return Matrix::checkInvertability(hits);
// }

void StraightLineMatrix::fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) const
{
  if (useTangens) {
    A(i,0)            = -xi;
    A(i,1)            = 1.;
    A(i,2)            = -k*xi;
    A(i,3)            = k;
  }
  else {
    A(i,0)            = 1.;
    A(i,1)            = -xi;
    A(i,2)            = k;
    A(i,3)            = -k*xi;
  }
}
