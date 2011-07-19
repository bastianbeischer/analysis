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

void CenteredBrokenLineMatrix2D::fillMatrixFromHit(TMatrixD& A, unsigned int i, double k, double c, double s) const
{
  int slopeXindex = k > 0. ? 2 : 3;
  int slopeYindex = k > 0. ? 4 : 5;

  A(i,0)            = c;
  A(i,1)            = -s;
  A(i,slopeXindex)  = k*c;
  A(i,slopeYindex)  = -k*s;
}
