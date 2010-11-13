#ifndef StraightLineMatrix_hh
#define StraightLineMatrix_hh

#include "Matrix.hh"

class StraightLineMatrix :
  public Matrix
{
  
public:
  StraightLineMatrix();
  ~StraightLineMatrix();
  
protected:
  void fillMatrixFromHit(unsigned int i, bool useTangens, float k, float xi);
  
};

#endif /* StraightLineMatrix_hh */
