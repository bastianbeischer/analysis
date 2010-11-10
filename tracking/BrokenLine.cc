#include "BrokenLine.hh"

#include <TVectorD.h>
#include <TMatrixD.h>

#include "Hit.hh"

#include <cmath>
#include <iostream>

BrokenLine::BrokenLine() :
  Track(),
  m_upperX0(0),
  m_lowerX0(0),
  m_upperSlopeX(0),
  m_lowerSlopeX(0),
  m_y0(0),
  m_slopeY(0)
{
}

BrokenLine::~BrokenLine()
{
}

int BrokenLine::fit(QVector<Hit*> hits)
{
  unsigned int nHits = hits.size();

  // basic dimensions of matrices
  unsigned int nRow = nHits;
  unsigned int nCol = 6;

  if (nRow < nCol) {
    // qDebug() << "Track::fit -- Can't fit: not enough hits!";    
    return 0;
  }

  // declare matrices for the calculation
  TMatrixD A(nRow,nCol);
  TVectorD b(nRow);
  TMatrixD U(nRow,nRow);
  TMatrixD CombineXandY(1,2);

  // this parameter is arbitrary. z0 = 0 should minimize correlations...
  float z0 = 0.0;

  for (unsigned int i = 0; i < nRow; i++) {

    Hit* hit = hits.at(i);

    TVector3 pos = hit->position();
    TVector3 counterPos = hit->counterPosition();

    // get information from detector...
    double angle = hit->angle();

    // HACK: because of different system of reference:
    // angle is currently the angle to the y axis and not to x. therefore:
    angle += M_PI/2.;
    
    // fill the matrices
    float k = pos.z() - z0;
    bool useTangens = fabs(angle) < M_PI/4. ? true : false;
    float xi = useTangens ? tan(angle) : 1./tan(angle);

    for (unsigned int j = 0; j < nCol; j++)
      A(i,j) = 0.;

    int offsetXindex = k>0 ? 0 : 1;
    int slopeXindex = k>0 ? 3 : 4;

    if (useTangens) {
      CombineXandY(0,0) = -xi;
      CombineXandY(0,1) = 1.;
      A(i,offsetXindex) = -xi;
      A(i,2)            = 1.;
      A(i,slopeXindex)  = -k*xi;
      A(i,5)            = k;
      b(i)              = -xi*pos.x() + pos.y();
    }
    else {
      CombineXandY(0,0) = 1.;
      CombineXandY(0,1) = -xi;
      A(i,offsetXindex) = 1.;
      A(i,2)            = -xi;
      A(i,slopeXindex)  = k;
      A(i,5)            = -k*xi;
      b(i)              = pos.x() - xi*pos.y();
    }

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
    if (hit->type() == Hit::tracker)
      sigmaV = 50e-3;
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
    // qDebug() << "Track::fit -- Aborting - Matrix U is singular!";
    return 0;
  }
  TMatrixD Uinv = U;
  Uinv.Invert();
  TMatrixD ATranspose(nCol,nRow);
  ATranspose.Transpose(A);
  TMatrixD M = ATranspose * Uinv * A;
  TVectorD c = ATranspose * Uinv * b;

  if (M.Determinant() == 0) {
    // qDebug() << "Track::fit -- Aborting - Matrix M is singular!";
    return 0;
  }
  TMatrixD Minv = M;
  Minv.Invert();
  TVectorD solution(nCol);
  solution = Minv * c;

  // calculate chi2 and track positions from fit parameters
  TMatrixD residuum(nRow,1);
  for (unsigned int i = 0; i < nRow; i++)
    residuum(i,0) = (A*solution - b)(i);
  TMatrixD residuumTrans(1,nRow);
  residuumTrans.Transpose(residuum);
  double chi2 = (residuumTrans * Uinv * residuum)(0,0);

  // return information from the fit.
  m_upperX0     = solution(0);
  m_lowerX0     = solution(1);
  m_y0          = solution(2);
  m_upperSlopeX = solution(3);
  m_lowerSlopeX = solution(4);
  m_slopeY      = solution(5);
  m_chi2        = chi2;
  m_ndf         = nRow-nCol;
  m_hits        = hits;

  if (m_verbose > 1) {
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << " results of straight line track fit: chi2/ndf      = " << m_chi2        << "/" << m_ndf << std::endl;
    std::cout << "                                       upperX0     = " << m_upperX0     << " mm" <<std::endl;
    std::cout << "                                       lowerX0     = " << m_lowerX0     << " mm" <<std::endl;
    std::cout << "                                       y0          = " << m_y0          << " mm" <<std::endl;
    std::cout << "                                       upperSlopeX = " << m_upperSlopeX << std::endl;
    std::cout << "                                       lowerSlopeX = " << m_lowerSlopeX << std::endl;
    std::cout << "                                       slopeY      = " << m_slopeY      << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
  }

  return 1;
}

double BrokenLine::x(double z) const
{
  double z_mid = (m_upperX0 - m_lowerX0) / (m_lowerSlopeX - m_upperSlopeX);

  if (z > z_mid)
    return m_upperX0 + z*m_upperSlopeX;
  return m_lowerX0 + z*m_lowerSlopeX;
}

double BrokenLine::y(double z) const
{
  return m_y0 + z*m_slopeY;
}
