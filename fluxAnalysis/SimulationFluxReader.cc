#include "SimulationFluxReader.hh"

#include "SimulationFlux.hh"
#include "Constants.hh"

#include <QFile>
#include <QDebug>
#include <QStringList>

#include <TFile.h>
#include <TROOT.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>

SimulationFluxReader* SimulationFluxReader::s_instance = 0;

SimulationFluxReader* SimulationFluxReader::instance()
{
  if (!s_instance)
    s_instance = new SimulationFluxReader();
  return s_instance;
}

SimulationFluxReader::SimulationFluxReader()
  : m_locations()
  , m_acceptances()
  , m_sources()
  , m_particles()
  , m_modulationParameters()
  , m_fluxes()
{
  QString dataPath = getenv("PERDAIXDATA_PATH");
  QString flightFileName = dataPath + "/fluxSimulation/flight.root";
  QString groundFileName = dataPath + "/fluxSimulation/groundKiruna.root";
  if (QFile::exists(flightFileName)) {
    if (m_locations.contains(SimulationFluxKey::Flight))
      m_locations.append(SimulationFluxKey::Flight);
    readKeys(flightFileName);
  }
  if (QFile::exists(groundFileName)) {
  if (m_locations.contains(SimulationFluxKey::GroundEsrange))
    m_locations.append(SimulationFluxKey::GroundEsrange);
    readKeys(groundFileName);
  }
}

void SimulationFluxReader::readKeys(const QString& fileName)
{
  TFile file(qPrintable(fileName));
  gROOT->cd();
  TList* keyList = file.GetListOfKeys();
  for (int i = 0; i < keyList->GetSize(); ++i) {
    Q_ASSERT(!strcmp(keyList->At(i)->ClassName(), "TKey"));
    TObject* object = static_cast<TKey*>(keyList->At(i))->ReadObj();
    if (!strcmp(object->ClassName(), "TH2D")) {
      QString name = QString(object->GetName());
      if (!name.contains("rigidity", Qt::CaseInsensitive) || name.contains("smeared", Qt::CaseInsensitive))
        continue;
      for (int albedo = 0; albedo < 2; ++albedo) {
        SimulationFluxKey key(object->GetName(), albedo);
        if (!m_locations.contains(key.location()))
          m_locations.append(key.location());
        if (!m_acceptances.contains(key.acceptance()))
          m_acceptances.append(key.acceptance());
        if (!m_sources.contains(key.source()))
          m_sources.append(key.source());
        if (!m_particles.contains(key.particle()))
          m_particles.append(key.particle());
        if (!m_modulationParameters.contains(key.modulationParameter()))
          m_modulationParameters.append(key.modulationParameter());
        TH2D* histogram = static_cast<TH2D*>(object);
        m_fluxes.insert(key, SimulationFlux(key, histogram).spectrum());
      }
    }
  }
  file.Close();
  qSort(m_acceptances);
  qSort(m_sources);
  qSort(m_modulationParameters);
}

SimulationFluxReader::~SimulationFluxReader()
{
  qDeleteAll(m_fluxes);
  s_instance = 0;
}

TH1D* SimulationFluxReader::spectrum(const SimulationFluxKey& key) const
{
  TH1D* flux = new TH1D(*m_fluxes[key]);
  if (!flux->GetSumw2())
    flux->Sumw2();
  return flux;
}

TH1D* SimulationFluxReader::spectrum(const QList<SimulationFluxKey>& keys) const
{
  TH1D* flux = spectrum(keys.at(0));
  if (!flux->GetSumw2())
    flux->Sumw2();
  QString title = "MC";
  title.append(" "+Particle(keys.at(0).particle()).name());
  for (int i = 1; i < keys.size(); i++) {
    SimulationFluxKey key = keys[i];
    flux->Add(m_fluxes[keys.at(i)]);
    title.append(" "+Particle(keys.at(i).particle()).name());
  }
  title.append(" sum");
  flux->SetTitle(qPrintable(title));
  flux->SetLineColor(kBlue - 2);
  flux->SetMarkerColor(kBlue - 2);
  return flux;
}

const QVector<SimulationFluxKey::Location>& SimulationFluxReader::locations() const
{
  return m_locations;
}

const QVector<SimulationFluxKey::Acceptance>& SimulationFluxReader::acceptances() const
{
  return m_acceptances;
}

const QVector<SimulationFluxKey::Source>& SimulationFluxReader::sources() const
{
  return m_sources;
}

const QVector<Particle::Type>& SimulationFluxReader::particles() const
{
  return m_particles;
}

const QVector<double>& SimulationFluxReader::modulationParameters() const
{
  return m_modulationParameters;
}
