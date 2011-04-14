#include "ParticleProperties.hh"

#include "Constants.hh"

ParticleProperties::ParticleProperties()
{
}

ParticleProperties::ParticleProperties(const Particle::Type& type)
{
  setType(type);
}

ParticleProperties::~ParticleProperties()
{
}

void ParticleProperties::setType(const Particle::Type& type)
{
  m_type = type;

  switch (m_type) {
  case Particle::Unknown:
    m_mass = 0.;
    m_charge = 0;
    m_pdgId = 0;
    m_name = "unknown";
    break;
  case Particle::Proton:
    m_mass = Constants::protonMass;
    m_charge = 1;
    m_pdgId = 2212;
    m_name = "proton";
    break;
  case Particle::Helium:
    m_mass = Constants::heliumMass;
    m_charge = 2;
    m_pdgId = 1000020040;
    m_name = "helium";
    break;
  case Particle::Electron:
    m_mass = Constants::electronMass;
    m_charge = -1;
    m_pdgId = 11;
    m_name = "e-";
    break;
  case Particle::Positron:
    m_mass = Constants::electronMass;
    m_charge = 1;
    m_pdgId = -11;
    m_name = "e+";
    break;
  case Particle::Muon:
    m_mass = Constants::muonMass;
    m_charge = -1;
    m_pdgId = 13;
    m_name = "mu-";
    break;
  case Particle::AntiMuon:
    m_mass = Constants::muonMass;
    m_charge = 1;
    m_pdgId = -13;
    m_name = "mu+";
    break;
  case Particle::PiPlus:
    m_mass = Constants::pionMass;
    m_charge = 1;
    m_pdgId = 211;
    m_name = "pi+";
    break;
  case Particle::PiMinus:
    m_mass = Constants::pionMass;
    m_charge = -1;
    m_pdgId = -211;
    m_name = "pi-";
    break;
  case Particle::Photon:
    m_mass = 0.;
    m_charge = 0.;
    m_pdgId = 22;
    m_name = "photon";
    break;
  default:
    break;
  }
}
