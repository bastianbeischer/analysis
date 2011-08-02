#include "SimulationFluxKey.hh"

#include <QFile>
#include <QDebug>
#include <QStringList>

#include <TFile.h>
#include <TObject.h>
#include <TROOT.h>

bool SimulationFluxKey::s_constructed = false;
QMap<Particle::Type, QString> SimulationFluxKey::s_particleNames;
QMap<SimulationFluxKey::Source, QString> SimulationFluxKey::s_sourceNames;

SimulationFluxKey::SimulationFluxKey(double phi, Particle::Type type, bool isAlbedo, Source source) :
m_phi(phi),
m_type(type),
m_isAlbedo(isAlbedo),
m_source(source)
{
  construct();
  if (!s_particleNames.contains(m_type)) {
    qFatal("SimulationFluxKey does not have this particle with name %s", qPrintable(Particle(m_type).name()));
  }
}

SimulationFluxKey::~SimulationFluxKey()
{

}

void SimulationFluxKey::construct()
{
  if (!s_constructed) {
    s_particleNames.insert(Particle::Photon, "gamma");
    s_particleNames.insert(Particle::Positron, "positron");
    s_particleNames.insert(Particle::Electron, "electron");
    s_particleNames.insert(Particle::AntiMuon, "mu+");
    s_particleNames.insert(Particle::Muon, "mu-");
    s_particleNames.insert(Particle::PiPlus, "pi+");
    s_particleNames.insert(Particle::PiMinus, "pi-");
    s_particleNames.insert(Particle::Proton, "proton");
    s_particleNames.insert(Particle::AntiProton, "antiproton");
    s_particleNames.insert(Particle::Helium, "alpha");

    s_sourceNames.insert(Total, "total");
    s_sourceNames.insert(Primary, "primary");
    s_sourceNames.insert(Secondary, "secondary");

    s_constructed = true;
  }
}

QString SimulationFluxKey::internalName()
{
  return s_particleNames[m_type];
}

QList<SimulationFluxKey::Source> SimulationFluxKey::allSources()
{
  construct();
  return s_sourceNames.keys();
}

QString SimulationFluxKey::sourceName(Source source)
{
  construct();
  return s_sourceNames[source];
}

QList<Particle::Type> SimulationFluxKey::allParticles()
{
  construct();
  return s_particleNames.keys();
}

QString SimulationFluxKey::name()
{
  QString name = Particle(m_type).name();
  if (m_isAlbedo) {
    name.prepend("albedo ");
  }
  return name;
}

QString SimulationFluxKey::sourceName()
{
  return s_sourceNames[m_source];
}

void SimulationFluxKey::setPhi(double phi)
{
  m_phi = phi;
}

void SimulationFluxKey::setSource(Source source)
{
  m_source = source;
}

SimulationFluxKey::Source SimulationFluxKey::source(QString sourceName)
{
  construct();
  return s_sourceNames.key(sourceName);
}

bool SimulationFluxKey::operator==(SimulationFluxKey key)
{
  return (m_phi == key.phi() && m_source == key.source() && m_type == key.type() && m_isAlbedo == key.isAlbedo());
}



