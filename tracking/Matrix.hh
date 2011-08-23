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
  
  void reset();
  int fit(const QVector<Hit*>& hits);
  double* solution() const {return m_solution;}
  unsigned int nRow() const {return m_nRow;}
  unsigned int nCol() const {return m_nCol;}
  double chi2() const {return m_chi2;}
  double ndf() const {return m_ndf;}
  
protected:
  virtual bool checkInvertability(const QVector<Hit*>& hits) const;
  virtual void fillMatrixFromHit(double* A, double k, double c, double s) const = 0;

protected:
  unsigned int m_nRow;
  unsigned int m_nCol;

  double m_chi2;
  double m_ndf;

  double* m_solution;

};

#endif /* Matrix_hh */
