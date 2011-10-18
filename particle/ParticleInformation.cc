#include "ParticleInformation.hh"

#include "Particle.hh"
#include "Track.hh"
#include "TimeOfFlight.hh"
#include "Hit.hh"

#include <QMap>
#include <QVector>
#include <cmath>
#include <cfloat>

ParticleInformation::ParticleInformation(const Particle* particle) :
  m_particle(particle),
  m_flags(None)
{
}

ParticleInformation::~ParticleInformation()
{
}

void ParticleInformation::process()
{
  m_flags = None;

  if (!m_particle || !m_particle->track())
    return;

  checkAllTrackerLayers();
  checkChi2Good();
  checkInsideMagnet();
  checkOutsideMagnet();
  checkHighTransverseRigidity();
  checkMagnetCollision();
  checkAlbedo();
  checkBetaGood();
}

void ParticleInformation::reset()
{
  m_flags = None;
  m_hitsInLayers.clear();
}

void ParticleInformation::checkAllTrackerLayers()
{
  const Track* track = m_particle->track();

  // count hits in each tracker layer
  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tracker) {
      double z = round(hit->position().z()*100.)/100.;
      m_hitsInLayers[z]++;
    }
  }

  // exactly 8 layers
  if (m_hitsInLayers.size() != 8)
    return;

  // exactly 1 count in each layer
  const QMap<double,int>::const_iterator endIt = m_hitsInLayers.end();
  for(QMap<double,int>::const_iterator it = m_hitsInLayers.begin(); it != endIt; ++it) {
    int count = it.value();
    if (count != 1)
      return;
  }

  m_flags |= AllTrackerLayers;
}

void ParticleInformation::checkChi2Good()
{
  const Track* track = m_particle->track();

  if (track->chi2() / track->ndf() > 5)
    return;
  m_flags |= Chi2Good;
}

void ParticleInformation::checkInsideMagnet()
{
  const Track* track = m_particle->track();

  if (!track->fitGood())
    return;
  double r1 = sqrt(pow(track->x(40.), 2.) + pow(track->y(40.), 2.));
  double r2 = sqrt(pow(track->x(-40.), 2.) + pow(track->y(-40.), 2.));
  if (r1 < 75 && r2 < 75)
    m_flags |= InsideMagnet;
}

void ParticleInformation::checkOutsideMagnet()
{
  const Track* track = m_particle->track();

  if (!track->fitGood())
    return;
  double r1 = sqrt(pow(track->x(40.), 2.) + pow(track->y(40.), 2.));
  double r2 = sqrt(pow(track->x(-40.), 2.) + pow(track->y(-40.), 2.));
  if (r1 > 120 && r2 > 120)
    m_flags |= OutsideMagnet;
}

void ParticleInformation::checkHighTransverseRigidity()
{
  const Track* track = m_particle->track();

  if (!track->fitGood())
    return;
  double transverseRigidity = track->transverseRigidity();
  // 2GeV currently...
  if (transverseRigidity > 2 || transverseRigidity == DBL_MAX)
    m_flags |= HighTransverseRigidity;
}

void ParticleInformation::checkMagnetCollision()
{
  const Track* track = m_particle->track();

  if (!track->fitGood())
    return;
  double x0 = track->x(0.);
  double y0 = track->y(0.);
  double r = sqrt(x0*x0 + y0*y0);
  if (r > 65 && r < 110)
    m_flags |= MagnetCollision;
}

void ParticleInformation::checkAlbedo()
{
  const Track* track = m_particle->track();

  if (!track->fitGood())
    return;
  if (m_particle->beta() < 0)
    m_flags |= Albedo;
}

void ParticleInformation::checkBetaGood()
{
  if (!m_particle->timeOfFlight())
    return;
  if (m_particle->timeOfFlight()->good())
    m_flags |= BetaGood;
}
