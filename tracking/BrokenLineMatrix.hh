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
  //bool checkInvertability(const QVector<Hit*>& hits) const;
  void fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) const;
  
};

#endif /* BrokenLineMatrix_hh */
