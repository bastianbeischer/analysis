#include "Particle.hh"

#include "Constants.hh"
#include "ParticleInformation.hh"

Particle::Particle() :
  m_type(None),
  m_information(new ParticleInformation(this)),
  m_track(0),
  m_mass(0.),
  m_charge(0)
{
}

Particle::Particle(Type type) :
  m_type(None),
  m_track(0),
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
