#include "Track.hh"

#include "Matrix.hh"
#include "Hit.hh"

#include "FieldManager.hh"
#include "MagneticField.hh"

#include <cmath>
#include <cfloat>
#include <iostream>

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
    const MagneticField* field = FieldManager::instance()->field();
    double B_estimate = field->fieldEstimate(); // tesla
    B_estimate *= 0.93;

    double z0_magnet = field->z0(); // mm
    double z1_magnet = field->z1(); // mm
    double x0_magnet = x(z0_magnet); // mm
    double x1_magnet = x(z1_magnet); // mm
    double L  = sqrt(pow(x1_magnet - x0_magnet, 2.) + pow(z1_magnet - z0_magnet,2.))*1000.; // convert from mm to m

    m_transverseRigidity = 0.3*B_estimate*L/alpha/1e6; // GeV
  }
}

TVector3 Track::meanFieldAlongTrack()
{
  const MagneticField* field = FieldManager::instance()->field();
  double z0 = field->z0();
  double z1 = field->z1();
  int nSteps = 1e2;

  double steplength = (z1 - z0) / nSteps;
  double meanB[3] = {0.,0.,0.};
  for (double z = z0; z <= z1; z += steplength) {
    double pos[3] = {x(z), y(z), z};
    double tempB[3];
    field->fieldValue(pos, tempB);
    for(int i = 0; i < 3; i++) meanB[i] += tempB[i];
  }

  for(int i = 0; i < 3; i++) meanB[i] /= nSteps;

  return TVector3(meanB[0], meanB[1], meanB[2]);
}

double Track::rigidity() const
{
  double theta = fabs(atan(slopeY(0.)));
  double rigidity = m_transverseRigidity/cos(theta);
  return rigidity;
}

