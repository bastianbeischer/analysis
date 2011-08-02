#include "SimulationFluxParticle.hh"

#include <QFile>
#include <QDebug>
#include <QStringList>

#include <TFile.h>
#include <TObject.h>
#include <TROOT.h>

bool SimulationFluxParticle::s_constructed = false;
QMap<Particle::Type, QString> SimulationFluxParticle::s_particleNames;
QMap<SimulationFluxParticle::Source, QString> SimulationFluxParticle::s_sourceNames;

SimulationFluxParticle::SimulationFluxParticle(Particle::Type type, bool isAlbedo, Source source) :
m_type(type),
m_isAlbedo(isAlbedo),
m_source(source)
{
  construct();
  if (!s_particleNames.contains(m_type)) {
    qFatal("SimulationFLuxParticle does not have this particle with name %s", qPrintable(Particle(m_type).name()));
  }
}

SimulationFluxParticle::~SimulationFluxParticle()
{

}

void SimulationFluxParticle::construct()
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

QString SimulationFluxParticle::internalName()
{
  return s_particleNames[m_type];
}

QList<SimulationFluxParticle::Source> SimulationFluxParticle::allSources()
{
  construct();
  return s_sourceNames.keys();
}

QString SimulationFluxParticle::sourceName(Source source)
{
  construct();
  return s_sourceNames[source];
}

QList<Particle::Type> SimulationFluxParticle::allParticles()
{
  construct();
  return s_particleNames.keys();
}

QString SimulationFluxParticle::name()
{
  QString name = Particle(m_type).name();
  if (m_isAlbedo) {
    name.prepend("albedo ");
  }
  return name;
}

QString SimulationFluxParticle::sourceName()
{
  return s_sourceNames[m_source];
}

SimulationFluxParticle::Source SimulationFluxParticle::source(QString sourceName)
{
  construct();
  return s_sourceNames.key(sourceName);
}

//SimulationFluxParticle SimulationFluxParticle::type(QString name, Source source)
//{
//  construct();
//  QList<Particle::Type> types = allParticles();
//  foreach(Particle::Type type, types) {
//    for(int isAlbedo = 0; isAlbedo < 2; isAlbedo++) {
//      SimulationFluxParticle currentParticle = SimulationFluxParticle(type, isAlbedo, source);
//      if (currentParticle.name() == name) {
//        return currentParticle;
//      }
//    }
//  }
//  qFatal("Particle not found");
//  return SimulationFluxParticle(Particle::Unknown, false, Total);
//}

bool SimulationFluxParticle::operator==(SimulationFluxParticle particle)
{
  bool isEqual = false;
  if (m_source == particle.source() && m_type == particle.type() && m_isAlbedo == particle.isAlbedo()) {
    isEqual = true;
  }

  return isEqual;
}

