#include "Track.hh"

#include "Matrix.hh"
#include "Hit.hh"

#include <cmath>
#include <cfloat>

Track::Track() :
  m_type(None),
  m_matrix(0),
  m_verbose(0),
  m_fitGood(0),
  m_chi2(0),
  m_ndf(0),
  m_transverseRigidity(0.)
{
}

Track::~Track()
{
}

int Track::fit(const QVector<Hit*>& hits)
{
  m_hits = hits;
  m_fitGood = m_matrix->fit(m_hits);
  if (m_fitGood) {
    retrieveFitResults();
    calculateTransverseRigidity();
  }
  return m_fitGood;
}

void Track::calculateTransverseRigidity()
{
  double alpha = bendingAngle();
  if (alpha == 0.)
    m_transverseRigidity = DBL_MAX;
  else {
    double z0_magnet = -40.; // mm
    double z1_magnet =  40.; // mm
    double x0_magnet = x(z0_magnet); // mm
    double x1_magnet = x(z1_magnet); // mm
    double B_estimate = 0.27; // tesla
    double L  = sqrt(pow(x1_magnet - x0_magnet, 2.) + pow(z1_magnet - z0_magnet,2.))*1000.; // convert from mm to m
    m_transverseRigidity = 0.3*B_estimate*L/alpha/1e6; // GeV
  }
}

double Track::rigidity() const
{
  double z = 100.;
  double sx = slopeX(z);
  double sy = slopeY(z);
  double rigidity = sqrt((sx*sx + sy*sy + 1)/(sx*sx + 1)) * m_transverseRigidity;
  return rigidity;
}
