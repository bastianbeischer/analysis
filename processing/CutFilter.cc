#include "CutFilter.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Constants.hh"

#include <math.h>

Cut::Cut(Type type) :
  m_type(type)
{
  m_minIsSet = false;
  m_maxIsSet = false;
}

Cut::~Cut() {
  
}

void Cut::setMin(double min) {
  m_minIsSet = true;
  m_min = min;
}

void Cut::setMax(double max) {
  m_maxIsSet = true;
  m_max = max;
}

bool Cut::passesCuts(double value) {
  if (m_minIsSet && m_maxIsSet) {
    if (m_min <= value && value <= m_max)
      return true;
  } else if (m_minIsSet) {
    if (m_min <= value)
      return true;
  } else if (m_maxIsSet) {
    if (value <= m_max)
      return true;
  }
  return false;
}

bool Cut::passes(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent* /*event*/) {
  if (m_type == rigidity) {
    const Track* track = particle->track();
    if (!track || !track->fitGood())
      return false;
    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
      return false;
    return passesCuts(fabs(track->rigidity()));
  }
  if (m_type == beta) {
    const Track* track = particle->track();
    if (!track || !track->fitGood())
      return false;
    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
      return false;
    return passesCuts(fabs(particle->beta()));
  }
  if (m_type == tofTimeOverThreshold) {
    const Track* track = particle->track();
    if (!track || !track->fitGood())
      return false;
    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
      return false;
    double totSum = 0.;
    int nTofHits = 0;
    const QVector<Hit*>::const_iterator endIt = clusters.end();
    for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
      Hit* hit = *clusterIt;
      if (hit->type() == Hit::tof) {
        TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
        double z = tofCluster->position().z();
        if (qAbs(track->x(z) - tofCluster->position().x()) > 0.95 * Constants::tofBarWidth / 2.)
          continue;
        std::vector<Hit*>& subHits = tofCluster->hits();
        std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
        for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
          TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(*it);
          totSum+= tofSipmHit->timeOverThreshold();
          ++nTofHits;
        }
      }
    }
    if (nTofHits > 0) {
      return passesCuts(totSum / nTofHits);
    }
    else {
      return false;
    }
  }
  if (m_type == trdDeposition) {
    const Track* track = particle->track();
    if (!track || !track->fitGood())
      return false;
    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
      return false;
    return passesCuts(sumOfSignalHeights(Hit::trd, track, clusters));
  }
  qFatal("m_type does not match any CutType::type!");
  return false;
}

double Cut::sumOfSignalHeights(const Hit::ModuleType type, const Track* /*track*/, const QVector<Hit*>& clusters) {
  double sumSignal = 0;
  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() == type) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        sumSignal+= (*it)->signalHeight();
      }
    }
  }
  return sumSignal;
}

CutFilter::CutFilter() {

}

CutFilter::~CutFilter() {

}

void CutFilter::addCut(Cut cut) {
  m_cuts.push_back(cut);
}

bool CutFilter::passes(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent* event) {
  bool passes = true;
  for (int i=0; i<m_cuts.size(); i++) {
    passes = passes && m_cuts[i].passes(clusters, particle, event);
  }
  return passes;
}

void CutFilter::setCuts(CutFilter cuts) {
  m_cuts = cuts.m_cuts;
}

  