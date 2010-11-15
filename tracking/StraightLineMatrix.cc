#include "StraightLineMatrix.hh"

#include <cmath>

StraightLineMatrix::StraightLineMatrix() :
  Matrix()
{
  m_nCol = 4;
}

StraightLineMatrix::~StraightLineMatrix()
{
}

void StraightLineMatrix::fillMatrixFromHit(unsigned int i, bool useTangens, float k, float xi)
{
  if (useTangens) {
    (*m_A)(i,0)            = -xi;
    (*m_A)(i,1)            = 1.;
    (*m_A)(i,2)            = -k*xi;
    (*m_A)(i,3)            = k;
  }
  else {
    (*m_A)(i,0)            = 1.;
    (*m_A)(i,1)            = -xi;
    (*m_A)(i,2)            = k;
    (*m_A)(i,3)            = -k*xi;
  }
}
