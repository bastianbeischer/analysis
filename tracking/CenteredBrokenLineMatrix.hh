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
  bool checkInvertability(const QVector<Hit*>& hits) const;
  void fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) const;
  
};

#endif /* CenteredBrokenLineMatrix_hh */
