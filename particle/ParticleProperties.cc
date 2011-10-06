#include "ParticleProperties.hh"

#include "Constants.hh"

ParticleProperties::ParticleProperties()
{
}

ParticleProperties::ParticleProperties(const Enums::Particle& type)
{
  setType(type);
}

ParticleProperties::~ParticleProperties()
{
}

void ParticleProperties::setType(const Enums::Particle& type)
{
  m_type = type;

  switch (m_type) {
  case Enums::NoParticle:
    m_mass = 0.;
    m_charge = 0;
    m_pdgId = 0;
    m_name = "unknown";
    m_color = kBlack;
    break;
  case Enums::Proton:
    m_mass = Constants::protonMass;
    m_charge = 1;
    m_pdgId = 2212;
    m_name = "proton";
    m_color = kBlue;
    break;
  case Enums::AntiProton:
    m_mass = Constants::protonMass;
    m_charge = -1;
    m_pdgId = -2212;
    m_name = "anti_proton";
    m_color = kOrange;
    break;
  case Enums::Helium:
    m_mass = Constants::heliumMass;
    m_charge = 2;
    m_pdgId = 1000020040;
    m_name = "helium";
    m_color = kMagenta;
    break;
  case Enums::Electron:
    m_mass = Constants::electronMass;
    m_charge = -1;
    m_pdgId = 11;
    m_name = "e-";
    m_color = kGreen;
    break;
  case Enums::Positron:
    m_mass = Constants::electronMass;
    m_charge = 1;
    m_pdgId = -11;
    m_name = "e+";
    m_color = kRed;
    break;
  case Enums::Muon:
    m_mass = Constants::muonMass;
    m_charge = -1;
    m_pdgId = 13;
    m_name = "mu-";
    m_color = kTeal-5;
    break;
  case Enums::AntiMuon:
    m_mass = Constants::muonMass;
    m_charge = 1;
    m_pdgId = -13;
    m_name = "mu+";
    m_color = kPink-3;
    break;
  case Enums::PiPlus:
    m_mass = Constants::pionMass;
    m_charge = 1;
    m_pdgId = 211;
    m_name = "pi+";
    m_color = kYellow+1;
    break;
  case Enums::PiMinus:
    m_mass = Constants::pionMass;
    m_charge = -1;
    m_pdgId = -211;
    m_name = "pi-";
    m_color = kViolet-6;
    break;
  case Enums::Photon:
    m_mass = 0.;
    m_charge = 0.;
    m_pdgId = 22;
    m_name = "photon";
    m_color = kGray;
    break;
  case Enums::Pi0:
    m_mass = Constants::pi0Mass;
    m_charge = 0;
    m_pdgId = 111;
    m_name = "pi0";
    m_color = kGray;
    break;
  case Enums::Higgs:
    m_mass = Constants::higgsMass;
    m_charge = 0;
    m_pdgId = 25;
    m_name = "Higgsi";
    m_color = kYellow;
    break;
  default:
    break;
  }
}
