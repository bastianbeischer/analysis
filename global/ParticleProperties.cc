#include "ParticleProperties.hh"
#include "Constants.hh"

#include <TColor.h>

#include <QString>

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

QString ParticleProperties::name() const
{
  switch (m_type) {
    case Enums::NoParticle: return "unknown";
    case Enums::Proton: return "proton";
    case Enums::AntiProton: return "anti_proton";
    case Enums::Helium: return "helium";
    case Enums::Electron: return "e-";
    case Enums::Positron: return "e+";
    case Enums::Muon: return "mu-";
    case Enums::AntiMuon: return "mu+";
    case Enums::PiPlus: return "pi+";
    case Enums::PiMinus: return "pi-";
    case Enums::Photon: return "photon";
    case Enums::Pi0: return "pi0";
    case Enums::Higgs: return "Higgsi";
    default: return 0;
  }
}

int ParticleProperties::color() const
{
  switch (m_type) {
    case Enums::NoParticle: return kBlack;
    case Enums::Proton: return kBlue;
    case Enums::AntiProton: return kOrange;
    case Enums::Helium: return kMagenta;
    case Enums::Electron: return kGreen;
    case Enums::Positron: return kRed;
    case Enums::Muon: return kTeal-5;
    case Enums::AntiMuon: return kPink-3;
    case Enums::PiPlus: return kYellow+1;
    case Enums::PiMinus: return kViolet-6;
    case Enums::Photon: return kGray;
    case Enums::Pi0: return kGray;
    case Enums::Higgs: return kYellow;
    default: return 0;
  }
}

void ParticleProperties::setType(const Enums::Particle& type)
{
  m_type = type;

  switch (m_type) {
  case Enums::NoParticle:
    m_mass = 0.;
    m_charge = 0;
    m_pdgId = 0;
    break;
  case Enums::Proton:
    m_mass = Constants::protonMass;
    m_charge = 1;
    m_pdgId = 2212;
    break;
  case Enums::AntiProton:
    m_mass = Constants::protonMass;
    m_charge = -1;
    m_pdgId = -2212;
    break;
  case Enums::Helium:
    m_mass = Constants::heliumMass;
    m_charge = 2;
    m_pdgId = 1000020040;
    break;
  case Enums::Electron:
    m_mass = Constants::electronMass;
    m_charge = -1;
    m_pdgId = 11;
    break;
  case Enums::Positron:
    m_mass = Constants::electronMass;
    m_charge = 1;
    m_pdgId = -11;
    break;
  case Enums::Muon:
    m_mass = Constants::muonMass;
    m_charge = -1;
    m_pdgId = 13;
    break;
  case Enums::AntiMuon:
    m_mass = Constants::muonMass;
    m_charge = 1;
    m_pdgId = -13;
    break;
  case Enums::PiPlus:
    m_mass = Constants::pionMass;
    m_charge = 1;
    m_pdgId = 211;
    break;
  case Enums::PiMinus:
    m_mass = Constants::pionMass;
    m_charge = -1;
    m_pdgId = -211;
    break;
  case Enums::Photon:
    m_mass = 0.;
    m_charge = 0.;
    m_pdgId = 22;
    break;
  case Enums::Pi0:
    m_mass = Constants::pi0Mass;
    m_charge = 0;
    m_pdgId = 111;
    break;
  case Enums::Higgs:
    m_mass = Constants::higgsMass;
    m_charge = 0;
    m_pdgId = 25;
    break;
  default:
    break;
  }
}
