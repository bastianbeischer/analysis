#include "StraightLine.hh"

#include "StraightLineMatrix.hh"
#include "TrackInformation.hh"
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
  m_type = Track::StraightLine;
  m_matrix = new StraightLineMatrix;
}

StraightLine::~StraightLine()
{
  delete m_matrix;
}

int StraightLine::fit(const QVector<Hit*>& hits)
{
  m_hits = hits;

  m_fitGood = m_matrix->fit(m_hits);

  if (m_fitGood != 0) {
    TVectorD solution = m_matrix->solution();
    
    // return information from the fit.
    m_x0      = solution(0);
    m_y0      = solution(1);
    m_slopeX  = solution(2);
    m_slopeY  = solution(3);
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

  return m_fitGood;
}
