#ifndef CenteredBrokenLineMatrix2D_hh
#define CenteredBrokenLineMatrix2D_hh

#include "Matrix.hh"

class CenteredBrokenLineMatrix2D :
  public Matrix
{
  
public:
  CenteredBrokenLineMatrix2D();
  virtual ~CenteredBrokenLineMatrix2D();
  
protected:
  void fillMatrixFromHit(double** A, unsigned int i, double k, double c, double s) const;
  
};

#endif /* CenteredBrokenLineMatrix2D_hh */
