#include "Matrix.hh"

#include <TMatrixD.h>

#include "Hit.hh"

#include <QMap>

#include <cmath>

Matrix::Matrix() :
  m_nRow(0),
  m_nCol(0),
  m_chi2(0),
  m_ndf(0),
  m_solution(0)
{
}

Matrix::~Matrix()
{
  delete [] m_solution;
}

void Matrix::reset()
{
  m_chi2 = 0.;
  m_ndf = 0;
  for (unsigned int i = 0; i < m_nCol; i++) {
    m_solution[i] = 0.;
  }
}

int Matrix::fit(const QVector<Hit*>& hits)
{
  QVector<Hit*> filteredHits;
  foreach(Hit* hit, hits)
    if (hit->type() != Hit::tof)
      filteredHits.append(hit);

  // basic dimensions of matrices
  m_nRow = filteredHits.size();

  if (!checkInvertability(filteredHits)) {
    return 0;
  }

  // declare matrices for the calculation
  double A[m_nRow][m_nCol];
  double b[m_nRow];
  double weights[m_nRow];
  double c[m_nCol];

  for (unsigned int i = 0; i < m_nRow; i++) {
    Hit* hit = filteredHits.at(i);

    const TVector3& pos = hit->position();
    const double& k = pos.z();
    const double& angle = -hit->angle();
    const double& sigmaU = hit->resolution();

    double c = cos(angle);
    double s = sin(angle);
    
    b[i] = c*pos.x() - s*pos.y();

    for (unsigned int j = 0; j < m_nCol; j++)
      A[i][j] = 0.;
    fillMatrixFromHit(A[i], k, c, s);

    weights[i] = 1./(sigmaU*sigmaU); // this is the inverse of the sigma for the i'th measurement
  } // loop over hits
  
  // calculate solution
  TMatrixD Minv(m_nCol, m_nCol);
  for (unsigned int i = 0; i < m_nCol; i++) {
    for (unsigned int j = 0; j < m_nCol; j++) {
      double entry = 0.;
      for (unsigned int k = 0; k < m_nRow; k++) {
        entry += A[k][i]*weights[k]*A[k][j]; // M = A^T * U^(-1) * A
      }
      Minv(i,j) = entry;
    }
  }
  Minv.InvertFast();

  // right hand side
  for (unsigned int i = 0; i < m_nCol; i++) {
    c[i] = 0.;
    for (unsigned int k = 0; k < m_nRow; k++)
      c[i] += A[k][i]*weights[k]*b[k]; // c = A^T * U^(-1) * b
  }

  // solution
  for (unsigned int i = 0; i < m_nCol; i++) {
    m_solution[i] = 0.;
    for (unsigned int j = 0; j < m_nCol; j++)
      m_solution[i] += Minv(i,j) * c[j]; // x = M^(-1) * c
  }

  // calculate chi2 and track positions from fit parameters
  m_chi2 = 0;
  for (unsigned int i = 0; i < m_nRow; i++) {
    double prediction = 0;
    for (unsigned int j = 0; j < m_nCol; j++)
      prediction += A[i][j]*m_solution[j];
    m_chi2 += pow(prediction - b[i], 2.) * weights[i];
  }

  m_ndf = m_nRow - m_nCol;

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
