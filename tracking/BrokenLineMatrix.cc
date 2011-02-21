#include "BrokenLineMatrix.hh"

BrokenLineMatrix::BrokenLineMatrix() :
  Matrix()
{
  m_nCol = 6;
  m_solution.ResizeTo(m_nCol);
}

BrokenLineMatrix::~BrokenLineMatrix()
{
}

void BrokenLineMatrix::fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) const
{
  int offsetXindex = k > 0. ? 0 : 1;
  int slopeXindex = k > 0. ? 3 : 4;

  if (useTangens) {
    A(i,offsetXindex) = -xi;
    A(i,2)            = 1.;
    A(i,slopeXindex)  = -k*xi;
    A(i,5)            = k;
  }
  else {
    A(i,offsetXindex) = 1.;
    A(i,2)            = -xi;
    A(i,slopeXindex)  = k;
    A(i,5)            = -k*xi;
  }
}
