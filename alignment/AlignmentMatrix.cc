#include "AlignmentMatrix.hh"

#include "Manager.hh"
#include "Strategy.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Parameters.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "millepede.h"

#include <cmath>

AlignmentMatrix::AlignmentMatrix() :
  EventDestination(),
  m_nGlobal(0),
  m_nLocal(0),
  m_globalDerivatives(0),
  m_localDerivatives(0)
{
}

AlignmentMatrix::~AlignmentMatrix()
{
  delete[] m_globalDerivatives;
  delete[] m_localDerivatives;
}

void AlignmentMatrix::init()
{
  Manager* manager = Manager::instance();
  Strategy* strategy = manager->strategy();

  m_nGlobal = strategy->numberOfGlobalParameters();
  m_nLocal  = strategy->numberOfLocalParameters();;

  delete[] m_globalDerivatives;
  delete[] m_localDerivatives;
  m_globalDerivatives = new float[m_nGlobal];
  m_localDerivatives = new float[m_nLocal];

  resetArrays();
}

void AlignmentMatrix::resetArrays()
{
  for (unsigned int i = 0; i < m_nGlobal; i++)
    m_globalDerivatives[i] = 0.;
  for (unsigned int i = 0; i < m_nLocal; i++)
    m_localDerivatives[i] = 0.;
}

void AlignmentMatrix::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::AllTrackerLayers) ||
       (flags & ParticleInformation::MagnetCollision) ) {
    return;
  }

  const QVector<Hit*>& hits = track->hits();
  
  foreach(Hit* hit, hits) {
    // use only tracker and trd hits for alignment
    if(hit->type() == Hit::tof)
      continue;

    // position
    TVector3 pos = hit->position();
    float fx = pos.x();
    float fy = pos.y();
    float fz = pos.z();
    float z0 = 0.0;
    float k  = fz - z0;

    // detector ID
    unsigned short detId = hit->elementId();
    unsigned int index = Manager::instance()->parameters()->indexForDetId(detId);

    // angle
    float angle = -hit->angle();

    float c = cosf(angle);
    float s = sinf(angle);

    float u = fx*c - fy*s;
    float sigmaU = hit->resolution();

    // derivative for Delta_u!
    m_globalDerivatives[index] = 1.;
      
    int offsetXindex = 0;
    int offsetYindex = 1;
    int slopeXindex = 2;
    int slopeYindex = 3;
    if (m_nLocal == 5) {
      slopeXindex = k>0 ? 2 : 3;
      slopeYindex = 4;
    }
    else if (m_nLocal == 6) {
      offsetXindex = k>0 ? 0 : 1;
      offsetYindex = 2;
      slopeXindex = k>0 ? 3 : 4;
      slopeYindex = 5;
    }

    m_localDerivatives[offsetXindex] = c;
    m_localDerivatives[offsetYindex] = -s;
    m_localDerivatives[slopeXindex] = k*c;
    m_localDerivatives[slopeYindex] = -k*s;

    EQULOC(m_globalDerivatives, m_localDerivatives, u, sigmaU);
  }
}
