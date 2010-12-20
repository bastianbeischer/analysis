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
  // bool checkInvertability(const QVector<Hit*>& hits) const;
  void fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) const;
  
};

#endif /* CenteredBrokenLineMatrix2D_hh */
