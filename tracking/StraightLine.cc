#include "StraightLine.hh"

#include "StraightLineMatrix.hh"
#include "Hit.hh"
#include "Constants.hh"

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
  m_type = Enums::StraightLine;
  m_matrix = new StraightLineMatrix;
}

StraightLine::~StraightLine()
{
  delete m_matrix;
}

void StraightLine::retrieveFitResults()
{
  double* solution = m_matrix->solution();
    
  // return information from the fit.
  m_x0      = solution[0];
  m_y0      = solution[1];
  m_slopeX  = solution[2];
  m_slopeY  = solution[3];
  m_chi2    = m_matrix->chi2();
  m_ndf     = m_matrix->ndf();

  if (m_verbose > 0) {
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << " results of straight line track fit: chi2/ndf = " << m_chi2   << "/" << m_ndf << std::endl;
    std::cout << "                                       x0     = " << m_x0     << " mm" <<std::endl;
    std::cout << "                                       y0     = " << m_y0     << " mm" <<std::endl;
    std::cout << "                                       slopeX = " << m_slopeX << std::endl;
    std::cout << "                                       slopeY = " << m_slopeY << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
  }
}

double StraightLine::trackLength() const
{
  const TVector3& upperPoint = position(Constants::upperTofPosition);
  const TVector3& lowerPoint = position(Constants::lowerTofPosition);
  return (upperPoint-lowerPoint).Mag();
}
