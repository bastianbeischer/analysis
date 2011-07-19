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
  void fillMatrixFromHit(TMatrixD& A, unsigned int i, double k, double c, double s) const;
  
};

#endif /* BrokenLineMatrix_hh */
