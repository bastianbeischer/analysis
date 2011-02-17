#include "CenteredBrokenLineMatrix2D.hh"

CenteredBrokenLineMatrix2D::CenteredBrokenLineMatrix2D() :
  Matrix()
{
  m_nCol = 6;
  m_solution.ResizeTo(m_nCol);
}

CenteredBrokenLineMatrix2D::~CenteredBrokenLineMatrix2D()
{
}

void CenteredBrokenLineMatrix2D::fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) const
{
  int slopeXindex = k > 0. ? 2 : 3;
  int slopeYindex = k > 0. ? 4 : 5;

  if (useTangens) {
    A(i,0)            = -xi;
    A(i,1)            = 1.;
    A(i,slopeXindex)  = -k*xi;
    A(i,slopeYindex)  = k;
  }
  else {
    A(i,0)            = 1.;
    A(i,1)            = -xi;
    A(i,slopeXindex)  = k;
    A(i,slopeYindex)   = -k*xi;
  }
}
