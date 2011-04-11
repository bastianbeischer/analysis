#include "ParticleIdentifier.hh"

#include "Particle.hh"
#include "TimeOfFlight.hh"

#include "TOFSipmHit.hh"
#include "TOFCluster.hh"

#include <QDebug>

ParticleIdentifier::ParticleIdentifier()
{
}

ParticleIdentifier::~ParticleIdentifier()
{
}

void ParticleIdentifier::identify(Particle* particle)
{
  // if we can't figure it out conclusively: use unknown
  particle->setType(Particle::Unknown);

  // keep a list of candidates
  m_candidates.clear();
  // for (int type = Particle::START; type != Particle::END; ++type)
  //   m_candidates.append((Particle::Type)type);

  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;

  const TimeOfFlight* tof = particle->timeOfFlight();

  bool albedo = tof->timeOfFlight() > 0 ? false: true;
  int chargeSign = track->rigidity() > 0 ? 1: -1;
  if (albedo) 
    chargeSign *= -1.;

  if (chargeSign > 0) {
    particle->setType(Particle::Proton);
    m_candidates.removeAll(Particle::Electron);
    m_candidates.removeAll(Particle::Muon);
  }
  else {
    particle->setType(Particle::Electron);
    m_candidates.removeAll(Particle::Proton);
    m_candidates.removeAll(Particle::Helium);
    m_candidates.removeAll(Particle::Positron);
    m_candidates.removeAll(Particle::AntiMuon);
  }

  // time over threshold
  double timeOverThreshold = 0.;
  int nTofHits = 0;
  const QVector<Hit*> hits = track->hits();
  const QVector<Hit*>::const_iterator clusterEndIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != clusterEndIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
      std::vector<Hit*>& subHits = tofCluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(*it);
        timeOverThreshold+= tofSipmHit->timeOverThreshold();
        ++nTofHits;
      }
    }
  }
  timeOverThreshold = timeOverThreshold/nTofHits;

  if (timeOverThreshold < 37)
    m_candidates.removeAll(Particle::Helium);
  else
    particle->setType(Particle::Helium);

  //qDebug() << m_candidates;
}
