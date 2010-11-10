#include "StraightLine.hh"

#include "Hit.hh"

#include <TGraphErrors.h>
#include <TF1.h>
#include <TVectorD.h>
#include <TMatrixD.h>

#include <cmath>

StraightLine::StraightLine() :
  Track(),
  m_x0(0),
  m_slopeX(0),
  m_y0(0),
  m_slopeY(0)
{
}

StraightLine::~StraightLine()
{
}

int StraightLine::fit(QVector<Hit*> hits)
{
  unsigned int nHits = hits.size();

  // basic dimensions of matrices
  unsigned int nRow = nHits;
  unsigned int nCol = 4;

  if (nRow < nCol) {
    // qDebug() << "StraightLine::fit -- Can't fit: not enough hits!";    
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

    if (useTangens) {
      CombineXandY(0,0) = -xi;
      CombineXandY(0,1) = 1.;
      A(i,0)            = -xi;
      A(i,1)            = 1.;
      A(i,2)            = -k*xi;
      A(i,3)            = k;
      b(i)              = -xi*pos.x() + pos.y();
    }
    else {
      CombineXandY(0,0) = 1.;
      CombineXandY(0,1) = -xi;
      A(i,0)            = 1.;
      A(i,1)            = -xi;
      A(i,2)            = k;
      A(i,3)            = -k*xi;
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
    // qDebug() << "StraightLine::fit -- Aborting - Matrix U is singular!";
    return 0;
  }
  TMatrixD Uinv = U;
  Uinv.Invert();
  TMatrixD ATranspose(nCol,nRow);
  ATranspose.Transpose(A);
  TMatrixD M = ATranspose * Uinv * A;
  TVectorD c = ATranspose * Uinv * b;

  if (M.Determinant() == 0) {
    // qDebug() << "StraightLine::fit -- Aborting - Matrix M is singular!";
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
  m_x0     = solution(0);
  m_y0     = solution(1);
  m_slopeX = solution(2);
  m_slopeY = solution(3);
  m_chi2   = chi2;
  m_ndf    = nRow-nCol;
  m_hits   = hits;

  if (m_verbose > 1) {
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << " results of straight line track fit: chi2/ndf = " << m_chi2   << "/" << m_ndf << std::endl;
    std::cout << "                                       x0     = " << m_x0     << " mm" <<std::endl;
    std::cout << "                                       y0     = " << m_y0     << " mm" <<std::endl;
    std::cout << "                                       slopeX = " << m_slopeX << std::endl;
    std::cout << "                                       slopeY = " << m_slopeY << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
  }

  return 1;
}

int StraightLine::fitTrd(QVector<Hit*> hits)
{
  QVector<Hit*> trdHits;
  foreach(Hit* hit, hits) {
    if (hit->type() == Hit::trd)
      trdHits.push_back(hit);
  }
  return fit2D(trdHits);
}

int StraightLine::fit2D(QVector<Hit*> hits)
{
  // fill graph with points
  TGraphErrors graph;
  int i = 0;
  foreach(Hit* hit, hits) {
    if (hit->type() == Hit::trd) {
      graph.SetPoint(i, hit->position().z(), hit->position().x());
      graph.SetPointError(i, 0, 6./sqrt(12));
      i++;
    }
    else if (hit->type() == Hit::tracker) {
      graph.SetPoint(i, hit->position().z(), hit->position().x());
      graph.SetPointError(i, 0, 1.);
      i++;
    }
  }
   
  // need at least 2 points to fit
  if (i < 2)
    return 0;
  
  graph.Fit("pol1", "Q");
  TF1* fit = graph.GetFunction("pol1");

  m_x0     = fit->GetParameter(0);
  m_slopeX = fit->GetParameter(1);
  m_chi2   = fit->GetChisquare();
  m_ndf    = fit->GetNDF();
  m_hits   = hits;

  return 1;
}
