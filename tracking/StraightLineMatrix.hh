#ifndef StraightLineMatrix_hh
#define StraightLineMatrix_hh

#include "Matrix.hh"

class StraightLineMatrix :
  public Matrix
{
  
public:
  StraightLineMatrix();
  virtual ~StraightLineMatrix();
  
protected:
  void fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi);
  
};

#endif /* StraightLineMatrix_hh */
