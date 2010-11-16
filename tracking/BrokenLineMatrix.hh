#ifndef BrokenLineMatrix_hh
#define BrokenLineMatrix_hh

#include "Matrix.hh"

class BrokenLineMatrix :
  public Matrix
{
  
public:
  BrokenLineMatrix();
  virtual ~BrokenLineMatrix();
  
protected:
  void fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi);
  
};

#endif /* BrokenLineMatrix_hh */
