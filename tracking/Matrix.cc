#include "Matrix.hh"

#include <TMatrixD.h>

#include "Hit.hh"

#include <QMap>

#include <cmath>

Matrix::Matrix() :
  m_nRow(0),
  m_nCol(0),
  m_solution(TVectorD(0))
{
}

Matrix::~Matrix()
{
}

void Matrix::reset()
{
  m_chi2 = 0.;
  m_ndf = 0;
  m_solution.Zero();
}

int Matrix::fit(const QVector<Hit*>& hits)
{
  QVector<Hit*> filteredHits;
  foreach(Hit* hit, hits)
    if (hit->type() != Hit::tof)
      filteredHits.append(hit);

  unsigned int nHits = filteredHits.size();

  // basic dimensions of matrices
  m_nRow = nHits;

  if (!checkInvertability(filteredHits)) {
    return 0;
  }

  // declare matrices for the calculation
  TMatrixD A(m_nRow, m_nCol);
  TVectorD solution(m_nCol);
  TVectorD b(m_nRow);
  TMatrixD Uinv(m_nRow,m_nRow);
  TMatrixD CombineXandY(1,2);

  for (unsigned int i = 0; i < m_nRow; i++) {
    Hit* hit = filteredHits.at(i);

    // this parameter is arbitrary. z0 = 0 should minimize correlations...
    double z0 = 0.0;

    TVector3 pos = hit->position();
    double k = pos.z() - z0;
    double angle = -hit->angle();
    double sigmaU = hit->resolution();

    double c = cos(angle);
    double s = sin(angle);
    
    b(i) = c*pos.x() - s*pos.y();

    for (unsigned int j = 0; j < m_nCol; j++)
      A(i,j) = 0.;
    fillMatrixFromHit(A, i, k, c, s);

    Uinv(i,i) = 1./(sigmaU*sigmaU); // this is the inverse of the sigma for the i'th measurement

  } // loop over hits
  
  // calculate solution
  TMatrixD ATranspose(m_nCol,m_nRow);
  ATranspose.Transpose(A);
  TMatrixD M = ATranspose * Uinv * A;
  TVectorD c = ATranspose * Uinv * b;

  TMatrixD Minv = M;
  Minv.InvertFast();

  solution = Minv * c;

  // calculate chi2 and track positions from fit parameters
  TMatrixD residuum(m_nRow,1);
  for (unsigned int i = 0; i < m_nRow; i++)
    residuum(i,0) = (A*solution - b)(i);
  TMatrixD residuumTrans(1,m_nRow);
  residuumTrans.Transpose(residuum);

  m_chi2 = (residuumTrans * Uinv * residuum)(0,0);
  m_ndf = m_nRow - m_nCol;
  m_solution = solution;

  return 1;
}

// currently more strict than "necessary"
bool Matrix::checkInvertability(const QVector<Hit*>& hits) const
{
  if(m_nRow < m_nCol)
    return false;

  QMap<double,int> anglesTop;
  QMap<double,int> anglesBottom;
  const QVector<Hit*>::const_iterator hitsEnd = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    double angle = round(hit->angle()*180./M_PI * 10.)/10.;
    if (hit->position().z() > 0.) {
      anglesTop[angle]++;
    }
    else {
      anglesBottom[angle]++;
    }
  }

  if (anglesTop.size() < 2 || anglesBottom.size() < 2)
    return false;

  return true;
}
