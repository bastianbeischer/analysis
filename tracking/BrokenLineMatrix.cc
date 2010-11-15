#include "BrokenLineMatrix.hh"

BrokenLineMatrix::BrokenLineMatrix() :
  Matrix()
{
  m_nCol = 6;
}

BrokenLineMatrix::~BrokenLineMatrix()
{
}

void BrokenLineMatrix::fillMatrixFromHit(unsigned int i, bool useTangens, float k, float xi)
{
  int offsetXindex = k > 0. ? 0 : 1;
  int slopeXindex = k > 0. ? 3 : 4;

  if (useTangens) {
    (*m_A)(i,offsetXindex) = -xi;
    (*m_A)(i,2)            = 1.;
    (*m_A)(i,slopeXindex)  = -k*xi;
    (*m_A)(i,5)            = k;
  }
  else {
    (*m_A)(i,offsetXindex) = 1.;
    (*m_A)(i,2)            = -xi;
    (*m_A)(i,slopeXindex)  = k;
    (*m_A)(i,5)            = -k*xi;
  }
}
