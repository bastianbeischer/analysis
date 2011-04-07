#include "Particle.hh"

#include "Constants.hh"
#include "ParticleInformation.hh"

#include "CenteredBrokenLine.hh"
#include "CenteredBrokenLine2D.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "TrackFinding.hh"

Particle::Particle() :
  m_type(None),
  m_track(new CenteredBrokenLine),
  m_information(new ParticleInformation(this)),
  m_mass(0.),
  m_charge(0)
{
}

Particle::Particle(Type type) :
  m_type(None),
  m_track(new CenteredBrokenLine),
  m_information(new ParticleInformation(this)),
  m_mass(0.),
  m_charge(0)
{
  setType(type);
}

Particle::~Particle()
{
  delete m_information;
}

void Particle::setType(Type type)
{
  switch (type) {
  case Proton:
    m_mass = Constants::protonMass;
    m_charge = 1;
    break;
  case Electron:
    m_mass = Constants::electronMass;
    m_charge = -1;
    break;
  case Positron:
    m_mass = Constants::electronMass;
    m_charge = 1;
    break;
  case Muon:
    m_mass = Constants::muonMass;
    m_charge = -1;
    break;
  case AntiMuon:
    m_mass = Constants::muonMass;
    m_charge = 1;
    break;
  case Helium:
    m_mass = Constants::heliumMass;
    m_charge = 2;
    break;
  case Pion:
    m_mass = Constants::pionMass;
    m_charge = 1;
    break;
  default:
    // shouldn't happen!
    Q_ASSERT(false);
    break;
  }
}

void Particle::setTrackType(Track::Type trackType)
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
