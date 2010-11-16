#ifndef Matrix_hh
#define Matrix_hh

#include <QVector>
#include <TVectorD.h>

class Hit;

class Matrix
{
  
public:
  Matrix();
  virtual ~Matrix();
  
  int fit(const QVector<Hit*>& hits);
  TVectorD solution() const {return m_solution;}
  unsigned int nRow() const {return m_nRow;}
  unsigned int nCol() const {return m_nCol;}
  double chi2() const {return m_chi2;}
  double ndf() const {return m_ndf;}
  
protected:
  virtual void fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) = 0;

protected:
  unsigned int m_nRow;
  unsigned int m_nCol;

  double m_chi2;
  double m_ndf;

  TVectorD m_solution;

};

#endif /* Matrix_hh */
