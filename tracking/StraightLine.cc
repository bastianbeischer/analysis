#include "StraightLine.hh"

#include "StraightLineMatrix.hh"
#include "Hit.hh"
#include "Setup.hh"

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

int StraightLine::fitTrd(const QVector<Hit*>& hits)
{
  m_hits = hits;

  QVector<Hit*> trdHits;
  foreach(Hit* hit, m_hits) {
    if (hit->type() == Hit::trd)
      trdHits.push_back(hit);
  }
  return fit2D(trdHits);
}

int StraightLine::fit2D(const QVector<Hit*>& hits)
{
  m_hits = hits;

  // fill graph with points
  TGraphErrors graph;
  int i = 0;
  foreach(Hit* hit, m_hits) {
    if (hit->type() == Hit::trd) {
      TVector3 pos = hit->position();
      graph.SetPoint(i, pos.z(), pos.x());
      graph.SetPointError(i, 0, 6./sqrt(12));
      i++;
    }
    else if (hit->type() == Hit::tracker) {
      TVector3 pos = hit->position();
      graph.SetPoint(i, pos.z(), pos.x());
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

  return 1;
}
