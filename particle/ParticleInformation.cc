#include "ParticleInformation.hh"

#include "Particle.hh"
#include "Track.hh"
#include "TimeOfFlight.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "Helpers.hh"

#include <QMap>
#include <QVector>
#include <cmath>
#include <cfloat>

ParticleInformation::ParticleInformation(const Particle* particle)
  : m_particle(particle)
  , m_flags(Enums::NoEventFlag)
{
}

ParticleInformation::~ParticleInformation()
{
}

void ParticleInformation::process()
{
  m_flags = Enums::NoEventFlag;

  if (!m_particle || !m_particle->track())
    return;

  checkAllTrackerLayers();
  checkChi2Good();
  checkMagnet();
  checkHighTransverseRigidity();
  checkAlbedo();
  checkBetaGood();
  checkTrackGood();
}

void ParticleInformation::reset()
{
  m_flags = Enums::NoEventFlag;
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

  m_flags |= Enums::AllTrackerLayers;
}

void ParticleInformation::checkChi2Good()
{
  const Track* track = m_particle->track();

  if (track && track->chi2() / track->ndf() < 5)
    m_flags |= Enums::Chi2Good;
}

void ParticleInformation::checkMagnet()
{
  const Track* track = m_particle->track();
  if (!track->fitGood())
    return;
  double innerRadius = Constants::magnetInnerRadius;
  double outerRadius = Constants::magnetOuterRadius;
  double halfHeight = Constants::magnetHeight / 2.;
  double upperDistance = Helpers::addQuad(track->x(+halfHeight), track->y(+halfHeight));
  double lowerDistance = Helpers::addQuad(track->x(-halfHeight), track->y(-halfHeight));
  if (upperDistance < innerRadius && lowerDistance < innerRadius) {
    m_flags |= Enums::InsideMagnet;
  } else if (outerRadius < upperDistance && outerRadius < lowerDistance) {
    m_flags |= Enums::OutsideMagnet;
  } else {
    m_flags |= Enums::MagnetCollision;
  }
}

void ParticleInformation::checkHighTransverseRigidity()
{
  const Track* track = m_particle->track();

  if (!track->fitGood())
    return;
  double transverseRigidity = track->transverseRigidity();
  // 2GeV currently...
  if (transverseRigidity > 2 || transverseRigidity == DBL_MAX)
    m_flags |= Enums::HighTransverseRigidity;
}

void ParticleInformation::checkAlbedo()
{
  const Track* track = m_particle->track();

  if (!track->fitGood())
    return;
  if (m_particle->timeOfFlight()->beta() < 0)
    m_flags |= Enums::Albedo;
}

void ParticleInformation::checkBetaGood()
{
  if (!m_particle->timeOfFlight())
    return;
  if (m_particle->timeOfFlight()->good())
    m_flags |= Enums::BetaGood;
}

void ParticleInformation::checkTrackGood()
{
  Track* track = m_particle->track();
  if (track && track->fitGood())
    m_flags |= Enums::TrackGood;
}
