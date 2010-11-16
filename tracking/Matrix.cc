#include "Matrix.hh"

#include <TMatrixD.h>

#include "Hit.hh"

#include <cmath>
#include <iostream>

Matrix::Matrix() :
  m_nRow(0),
  m_nCol(0),
  m_solution(TVectorD(0))
{
}

Matrix::~Matrix()
{
}

int Matrix::fit(const QVector<Hit*>& hits)
{
  unsigned int nHits = hits.size();

  // basic dimensions of matrices
  m_nRow = nHits;

  if (m_nRow < m_nCol) {
    //std::cerr << "Matrix::fit -- Can't fit: not enough hits!" << std::endl;
    return 0;
  }

  // declare matrices for the calculation
  TMatrixD A(m_nRow, m_nCol);
  TVectorD solution(m_nCol);
  TVectorD b(m_nRow);
  TMatrixD U(m_nRow,m_nRow);
  TMatrixD CombineXandY(1,2);

  for (unsigned int i = 0; i < m_nRow; i++) {
    Hit* hit = hits.at(i);

    TVector3 pos = hit->position();

    // get information from detector...
    double angle = hit->angle();
    angle += M_PI/2.;

    // fill matrix
    // this parameter is arbitrary. z0 = 0 should minimize correlations...
    float z0 = 0.0;
    float k = hit->position().z() - z0;
    bool useTangens = fabs(angle) < M_PI/4. ? true : false;
    float xi = useTangens ? tan(angle) : 1./tan(angle);

    if (useTangens) {
      CombineXandY(0,0) = -xi;
      CombineXandY(0,1) = 1.;
      b(i)              = -xi*pos.x() + pos.y();
    }
    else {
      CombineXandY(0,0) = 1.;
      CombineXandY(0,1) = -xi;
      b(i)              = pos.x() - xi*pos.y();
    }

    for (unsigned int j = 0; j < m_nCol; j++)
      A(i,j) = 0.;
    fillMatrixFromHit(A, i, useTangens, k, xi);

    // calculate covariance matrix

    // Rot is the matrix that maps u,v, to x,y (i.e. the backward rotation)
    TMatrixD Rot(2,2); 
    Rot(0,0) = cos(angle);
    Rot(0,1) = sin(angle);
    Rot(1,0) = -sin(angle);
    Rot(1,1) = cos(angle);
    TMatrixD RotTrans(2,2);
    RotTrans.Transpose(Rot);

    double sigmaV = hit->resolutionEstimate();
    TMatrixD V1(2,2);
    V1(0,0) = 0.;
    V1(0,1) = 0.;
    V1(1,0) = 0.;
    V1(1,1) = sigmaV*sigmaV;

    TMatrixD V2(2,2);
    V2 = Rot * V1 * RotTrans;

    TMatrixD CombineXandYTrans(2,1);
    CombineXandYTrans.Transpose(CombineXandY);

    TMatrixD V3 = TMatrixD(1,1);
    V3 = CombineXandY * V2 * CombineXandYTrans;

    U(i,i) = V3(0,0); // this is the sigma for the i'th measurement

  } // loop over hits
  
  
  // calculate solution

  // U is diagonal, inverse should always exist unless someone specified sigma = inf or something strange
  if (U.Determinant() == 0) {
    //std::cerr << "Matrix::fit -- U is singular!" << std::endl;
    return 0;
  }
  TMatrixD Uinv = U;
  Uinv.Invert();
  TMatrixD ATranspose(m_nCol,m_nRow);
  ATranspose.Transpose(A);
  TMatrixD M = ATranspose * Uinv * A;
  TVectorD c = ATranspose * Uinv * b;

  if (M.Determinant() == 0) {
    //std::cerr << "Matrix::fit -- M is singular!" << std::endl;
    return 0;
  }
  TMatrixD Minv = M;
  Minv.Invert();

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
