#include "ParticleIdentifier.hh"

#include "Particle.hh"
#include "TimeOfFlight.hh"

#include "TOFSipmHit.hh"
#include "TOFCluster.hh"

#include <iostream>

ParticleIdentifier::ParticleIdentifier()
{
  for (int type = Particle::START; type <= Particle::END; type = type<<1) {
    m_allParticles.append(new Particle((Particle::Type)type));
  }
}

ParticleIdentifier::~ParticleIdentifier()
{
  qDeleteAll(m_allParticles);
}

void ParticleIdentifier::identify(Particle* particle)
{
  // if we can't figure it out conclusively: use unknown
  particle->setType(Particle::Unknown);

  // keep a list of candidates
  m_candidates = m_allParticles;

  const Track* track = particle->track();
  const TimeOfFlight* tof = particle->timeOfFlight();
  if (!track || !track->fitGood())
    return;

  // calculate charge sign
  bool albedo = tof->timeOfFlight() > 0 ? false: true;
  int chargeSign = track->rigidity() > 0 ? 1: -1;
  if (albedo) 
    chargeSign *= -1;

  // remove candidates according to charge sign and set particle type to most likely value
  if (chargeSign > 0) {
    particle->setType(Particle::Proton);
    foreach(const Particle* candidate, m_candidates) {
      if (candidate->charge() <= 0)
        m_candidates.removeAll(candidate);
    }
  }
  else {
    particle->setType(Particle::Muon);
    foreach(const Particle* candidate, m_candidates) {
      if (candidate->charge() >= 0)
        m_candidates.removeAll(candidate);
    }
  }

  // time over threshold for helium ID
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

  // hard cut on tot for helium
  if (nTofHits > 0) {
    timeOverThreshold = timeOverThreshold/nTofHits;
    if (timeOverThreshold > 37 && chargeSign > 0)
      particle->setType(Particle::Helium);
    else {
      foreach(const Particle* candidate, m_candidates) {
        if (candidate->type() == Particle::Helium)
          m_candidates.removeAll(candidate);
      }
    }
  }

  // if only one candidate remains, use it.
  if (m_candidates.size() == 1)
    particle->setType(m_candidates.at(0)->type());
}
