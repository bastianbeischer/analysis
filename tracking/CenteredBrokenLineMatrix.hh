#ifndef CenteredBrokenLineMatrix_hh
#define CenteredBrokenLineMatrix_hh

#include "Matrix.hh"

class CenteredBrokenLineMatrix :
  public Matrix
{
  
public:
  CenteredBrokenLineMatrix();
  virtual ~CenteredBrokenLineMatrix();
  
protected:
  void fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi);
  
};

#endif /* CenteredBrokenLineMatrix_hh */
