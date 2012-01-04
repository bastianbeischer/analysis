#include "SimulationFluxKey.hh"

#include <QStringList>
#include <QDebug>

QMap<SimulationFluxKey::Location, QString> SimulationFluxKey::s_locationNames;
QMap<SimulationFluxKey::Acceptance, QString> SimulationFluxKey::s_acceptanceNames;
QMap<SimulationFluxKey::Source, QString> SimulationFluxKey::s_sourceNames;
QMap<Enums::Particle, QString> SimulationFluxKey::s_particleNames;

SimulationFluxKey::SimulationFluxKey(const QString& title, bool isAlbedo)
  : m_location(UndefinedLocation)
  , m_acceptance(UndefinedAcceptance)
  , m_source(UndefinedSource)
  , m_particle(Enums::NoParticle)
  , m_modulationParameter(-1)
  , m_isAlbedo(isAlbedo)
{
  construct();
  if (title.contains("corsika", Qt::CaseInsensitive))
    m_location = GroundEsrange;
  else if (title.contains("planetocosmics", Qt::CaseInsensitive))
    m_location = Flight;

  if (title.contains("perdaixTof", Qt::CaseInsensitive))
    m_acceptance = TofAcceptance;
  else if (title.contains("perdaix", Qt::CaseInsensitive))
    m_acceptance = InsideMagnetAcceptance;
  else
    m_acceptance = TwoPiAcceptance;

  QMapIterator<Enums::Particle, QString> iterator(s_particleNames);
  while (iterator.hasNext()) {
    iterator.next();
    if (title.contains(iterator.value()))
      m_particle = iterator.key();
  }

  if (title.contains("total"))
    m_source = TotalSource;
  else if (title.contains("primary"))
    m_source = PrimarySource;
  else if (title.contains("secondary"))
    m_source = SecondarySource;

  QString phiString = QString(title).split("phi_").at(1).split("_MV").at(0);
  m_modulationParameter = phiString.toDouble();
}

SimulationFluxKey::SimulationFluxKey(Location location, Acceptance acceptance, Source source, Enums::Particle particle, double phi, bool isAlbedo)
  : m_location(location)
  , m_acceptance(acceptance)
  , m_source(source)
  , m_particle(particle)
  , m_modulationParameter(phi)
  , m_isAlbedo(isAlbedo)
{
  construct();
  if (!s_particleNames.contains(m_particle)) {
    qFatal("SimulationFluxKey does not have this particle with name %s", qPrintable(Particle(m_particle).name()));
  }
}

SimulationFluxKey::~SimulationFluxKey()
{
}

void SimulationFluxKey::construct()
{
  if (!s_locationNames.count()) {
    s_locationNames.insert(GroundEsrange, "Ground Esrange");
    s_locationNames.insert(Flight, "Flight");

    s_acceptanceNames.insert(InsideMagnetAcceptance, "perdaix");
    s_acceptanceNames.insert(TofAcceptance, "perdaixTof");
    s_acceptanceNames.insert(TwoPiAcceptance, "twoPi");

    s_particleNames.insert(Enums::Photon, "gamma");
    s_particleNames.insert(Enums::Positron, "positron");
    s_particleNames.insert(Enums::Electron, "electron");
    s_particleNames.insert(Enums::AntiMuon, "mu+");
    s_particleNames.insert(Enums::Muon, "mu-");
    s_particleNames.insert(Enums::PiPlus, "pi+");
    s_particleNames.insert(Enums::PiMinus, "pi-");
    s_particleNames.insert(Enums::Pi0, "pi0");
    s_particleNames.insert(Enums::Proton, "proton");
    s_particleNames.insert(Enums::AntiProton, "antiproton");
    s_particleNames.insert(Enums::Helium, "alpha");

    s_sourceNames.insert(TotalSource, "total");
    s_sourceNames.insert(PrimarySource, "primary");
    s_sourceNames.insert(SecondarySource, "secondary");
  }
}

QString SimulationFluxKey::locationName() const
{
  return s_locationNames[m_location];
}

QString SimulationFluxKey::acceptanceName() const
{
  return s_acceptanceNames[m_acceptance];
}

QString SimulationFluxKey::sourceName() const
{
  return s_sourceNames[m_source];
}

QString SimulationFluxKey::particleName() const
{
  QString name = Particle(m_particle).name();
  if (m_isAlbedo)
    name.prepend("albedo ");
  return name;
}

QString SimulationFluxKey::modulationParameterName() const
{
  return QString("%1 MV").arg(m_modulationParameter);
}

void SimulationFluxKey::setModulationParameter(double phi)
{
  m_modulationParameter = phi;
}

void SimulationFluxKey::setSource(Source source)
{
  m_source = source;
}

SimulationFluxKey::Location SimulationFluxKey::location(const QString& locationName)
{
  construct();
  return s_locationNames.key(locationName);
}

QString SimulationFluxKey::locationName(Location location)
{
  construct();
  return s_locationNames[location];
}

SimulationFluxKey::Acceptance SimulationFluxKey::acceptance(const QString& acceptanceName)
{
  construct();
  return s_acceptanceNames.key(acceptanceName);
}

QString SimulationFluxKey::acceptanceName(Acceptance acceptance)
{
  construct();
  return s_acceptanceNames[acceptance];
}

SimulationFluxKey::Source SimulationFluxKey::source(const QString& sourceName)
{
  construct();
  return s_sourceNames.key(sourceName);
}

QString SimulationFluxKey::sourceName(Source source)
{
  construct();
  return s_sourceNames[source];
}

Enums::Particle SimulationFluxKey::particle(const QString& particleName)
{
  construct();
  return s_particleNames.key(particleName, Enums::NoParticle);
}

QString SimulationFluxKey::particleName(Enums::Particle particle)
{
  construct();
  return s_particleNames[particle];
}

double SimulationFluxKey::modulationParameter(const QString& phiName)
{
  construct();
  return phiName.left(phiName.indexOf(" MV")).toDouble();
}

QString SimulationFluxKey::modulationParameterName(double phi)
{
  construct();
  return QString("%1 MV").arg(phi);
}

bool SimulationFluxKey::operator==(const SimulationFluxKey& key)
{
  return (m_modulationParameter == key.modulationParameter() && m_source == key.source() && m_particle == key.particle() && m_isAlbedo == key.isAlbedo());
}

bool operator<(const SimulationFluxKey& le, const SimulationFluxKey& ri)
{
  if (!qFuzzyCompare(le.modulationParameter(), ri.modulationParameter())) {
    return le.modulationParameter() < ri.modulationParameter();
  } else {
    if (le.source() != ri.source()) {
      return le.source() < ri.source();
    } else {
      if (le.particle() != ri.particle()) {
        return le.particle() < ri.particle();
      } else {
        if (le.location() != ri.location()) {
          return le.location() < ri.location();
        } else {
          if (le.acceptance() != ri.acceptance()) {
            return le.acceptance() < ri.acceptance();
          } else {
            return le.isAlbedo() < ri.isAlbedo();
          }
        }
      }
    }
  }
}

QDebug operator<<(QDebug debug, const SimulationFluxKey& key)
{
  debug
    << key.locationName() << key.acceptanceName() << key.sourceName()
    << key.particleName() << key.modulationParameterName();
  if (key.isAlbedo())
    debug << "albedo";
  return debug;
}
