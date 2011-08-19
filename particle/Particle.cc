#include "Particle.hh"

#include "ParticleDB.hh"
#include "ParticleProperties.hh"
#include "ParticleInformation.hh"
#include "TimeOfFlight.hh"
#include "TRDReconstruction.hh"
#include "Constants.hh"

#include "CenteredBrokenLine.hh"
#include "CenteredBrokenLine2D.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "TrackFinding.hh"

Particle::Particle()
{
  init();
}

Particle::Particle(const Type& type)
{
  init();
  setType(type);
}

Particle::Particle(const int& pdgId)
{
  init();
  setPdgId(pdgId);
}

Particle::~Particle()
{
  delete m_track;
  delete m_tof;
  delete m_trd;
  delete m_information;
}

void Particle::init()
{
  m_properties = ParticleDB::instance()->lookupType(Unknown);
  m_track = new CenteredBrokenLine;
  m_tof = new TimeOfFlight;
  m_trd = new TRDReconstruction;
  m_information = new ParticleInformation(this);
}

void Particle::reset()
{
  m_properties = ParticleDB::instance()->lookupType(Unknown);
  if (m_track)
    m_track->reset();
  m_tof->reset();
  m_trd->reset();
  m_information->reset();
}

void Particle::setType(const Type& type)
{
  m_properties = ParticleDB::instance()->lookupType(type);
}

void Particle::setPdgId(const int& pdgId)
{
  m_properties = ParticleDB::instance()->lookupPdgId(pdgId);
}

void Particle::setTrackType(const Track::Type& trackType)
{
  if (m_track)
    delete m_track;

  if (trackType == Track::None)
    m_track = 0;
  if (trackType == Track::CenteredBrokenLine)
    m_track = new CenteredBrokenLine;
  else if (trackType == Track::CenteredBrokenLine2D)
    m_track = new CenteredBrokenLine2D;
  else if (trackType == Track::BrokenLine)
    m_track = new BrokenLine;
  else if (trackType == Track::StraightLine)
    m_track = new StraightLine;
}

double Particle::transverseMomentum() const
{
  if (m_track)
    return m_properties->charge()*m_track->transverseRigidity();
  return 0.;
}

double Particle::momentum() const
{
  if (m_track)
    return m_properties->charge()*m_track->rigidity();
  return 0.;
}

double Particle::beta() const
{
  if (m_track)
    return m_track->trackLength() / (m_tof->timeOfFlight() * Constants::speedOfLight);
  return 0.;
}

//////////////////////////////////////////////////
// loopthrough functions for convenience

Particle::Type Particle::type() const
{
  return m_properties->type();
}

int Particle::pdgId() const
{
  return m_properties->pdgId();
}

QString Particle::name() const
{
  return m_properties->name();
}

double Particle::mass() const
{
  return m_properties->mass();
}

int Particle::charge() const
{
  return m_properties->charge();
}

Color_t Particle::color() const
{
  return m_properties->color();
}
