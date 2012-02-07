#include "ParticleIdentifier.hh"

#include "Particle.hh"
#include "ParticleDB.hh"
#include "ParticleProperties.hh"
#include "TimeOfFlight.hh"

#include "TOFSipmHit.hh"
#include "TOFCluster.hh"

#include <iostream>

ParticleIdentifier::ParticleIdentifier()
{
}

ParticleIdentifier::~ParticleIdentifier()
{
}

void ParticleIdentifier::identify(Particle* particle)
{
  // if we can't figure it out conclusively: use unknown
  particle->setType(Enums::NoParticle);

  // keep a list of candidates
  m_candidates = ParticleDB::instance()->allParticles();

  const Track* track = particle->track();
  const TimeOfFlight* tof = particle->timeOfFlight();
  if (!track || !track->fitGood())
    return;

  // calculate charge sign
  bool albedo = tof->timeOfFlight() > 0 ? false: true;
  int chargeSign = 0;//track->rigidity() > 0 ? 1: -1;
  if (albedo)
    chargeSign *= -1;

  // remove candidates according to charge sign and set particle type to most likely value
  if (chargeSign > 0) {
    particle->setType(Enums::Proton);
    foreach(const ParticleProperties* candidate, m_candidates) {
      if (candidate->charge() <= 0)
        m_candidates.removeAll(candidate);
    }
  }
  else {
    particle->setType(Enums::Muon);
    foreach(const ParticleProperties* candidate, m_candidates) {
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
      const std::vector<Hit*>& subHits = tofCluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(*it);
        timeOverThreshold+= tofSipmHit->timeOverThreshold();
        ++nTofHits;
      }
    }
  }

  double sumTRD = 0;
  for (QVector<Hit*>::const_iterator clusterIt = hits.begin(); clusterIt != clusterEndIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() == Hit::trd) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      const std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        sumTRD += (*it)->signalHeight();
      }
    }
  }

  // hard cut on tot for helium
  if (nTofHits > 0) {
    timeOverThreshold = timeOverThreshold/nTofHits;
    if (timeOverThreshold > 37 && sumTRD > 250 && chargeSign > 0)
      particle->setType(Enums::Helium);
    else {
      foreach(const ParticleProperties* candidate, m_candidates) {
        if (candidate->type() == Enums::Helium)
          m_candidates.removeAll(candidate);
      }
    }
  }

  // if only one candidate remains, use it.
  if (m_candidates.size() == 1)
    particle->setType(m_candidates.at(0)->type());
}
