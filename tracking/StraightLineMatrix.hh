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
  void fillMatrixFromHit(double** A, unsigned int i, double k, double c, double s) const;
  
};

#endif /* StraightLineMatrix_hh */
