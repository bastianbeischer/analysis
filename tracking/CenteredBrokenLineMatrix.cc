#include "CenteredBrokenLineMatrix.hh"

CenteredBrokenLineMatrix::CenteredBrokenLineMatrix() :
  Matrix()
{
  m_nCol = 5;
}

CenteredBrokenLineMatrix::~CenteredBrokenLineMatrix()
{
}

void CenteredBrokenLineMatrix::fillMatrixFromHit(unsigned int i, bool useTangens, float k, float xi)
{
  int slopeXindex = k > 0. ? 2 : 3;

  if (useTangens) {
    (*m_A)(i,0)            = -xi;
    (*m_A)(i,1)            = 1.;
    (*m_A)(i,slopeXindex)  = -k*xi;
    (*m_A)(i,4)            = k;
  }
  else {
    (*m_A)(i,0)            = 1.;
    (*m_A)(i,1)            = -xi;
    (*m_A)(i,slopeXindex)  = k;
    (*m_A)(i,4)            = -k*xi;
  }
}
