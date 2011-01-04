#include "CenteredBrokenLine.hh"

#include <TVectorD.h>
#include <TMatrixD.h>

#include "CenteredBrokenLineMatrix.hh"
#include "TrackInformation.hh"
#include "Hit.hh"

#include <cmath>
#include <iostream>

CenteredBrokenLine::CenteredBrokenLine() :
  Track(),
  m_x0(0),
  m_upperSlopeX(0),
  m_lowerSlopeX(0),
  m_y0(0),
  m_slopeY(0),
  m_zIntersection(0.)
{
  m_type = Track::CenteredBrokenLine;
  m_matrix = new CenteredBrokenLineMatrix;
}

CenteredBrokenLine::~CenteredBrokenLine()
{
  delete m_matrix;
}

int CenteredBrokenLine::fit(const QVector<Hit*>& hits)
{
  m_hits = hits;

  m_fitGood = m_matrix->fit(m_hits);

  if (m_fitGood != 0) {
    TVectorD solution = m_matrix->solution();

    // return information from the fit.
    m_x0            = solution(0);
    m_y0            = solution(1);
    m_upperSlopeX   = solution(2);
    m_lowerSlopeX   = solution(3);
    m_slopeY        = solution(4);
    m_chi2          = m_matrix->chi2();
    m_ndf           = m_matrix->ndf();

    if (m_verbose > 0) {
      std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
      std::cout << " results of straight line track fit: chi2/ndf      = " << m_chi2        << "/" << m_ndf << std::endl;
      std::cout << "                                       X0          = " << m_x0          << " mm" <<std::endl;
      std::cout << "                                       y0          = " << m_y0          << " mm" <<std::endl;
      std::cout << "                                       upperSlopeX = " << m_upperSlopeX << std::endl;
      std::cout << "                                       lowerSlopeX = " << m_lowerSlopeX << std::endl;
      std::cout << "                                       slopeY      = " << m_slopeY      << std::endl;
      std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
    }
  }

  m_information->process();

  return m_fitGood;
}

double CenteredBrokenLine::x(double z) const
{
  if (z > m_zIntersection)
    return m_x0 + z*m_upperSlopeX;
  return m_x0 + z*m_lowerSlopeX;
}

double CenteredBrokenLine::y(double z) const
{
  return m_y0 + z*m_slopeY;
}

double CenteredBrokenLine::slopeX(double z) const
{
  if (z > m_zIntersection)
    return m_upperSlopeX;
  return m_lowerSlopeX;
}

double CenteredBrokenLine::slopeY(double) const
{
  return m_slopeY;
}
