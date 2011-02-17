#include "CenteredBrokenLineMatrix.hh"

CenteredBrokenLineMatrix::CenteredBrokenLineMatrix() :
  Matrix()
{
  m_nCol = 5;
  m_solution.ResizeTo(m_nCol);
}

CenteredBrokenLineMatrix::~CenteredBrokenLineMatrix()
{
}

void CenteredBrokenLineMatrix::fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) const
{
  int slopeXindex = k > 0. ? 2 : 3;

  if (useTangens) {
    A(i,0)            = -xi;
    A(i,1)            = 1.;
    A(i,slopeXindex)  = -k*xi;
    A(i,4)            = k;
  }
  else {
    A(i,0)            = 1.;
    A(i,1)            = -xi;
    A(i,slopeXindex)  = k;
    A(i,4)            = -k*xi;
  }
}
