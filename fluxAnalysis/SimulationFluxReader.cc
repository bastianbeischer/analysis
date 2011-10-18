#include "SimulationFluxReader.hh"

#include "SimulationFlux.hh"
#include "Constants.hh"
#include "Helpers.hh"

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
  : m_fluxes()
{
  QString dataPath = Helpers::dataPath();
  QString flightFileName = dataPath + "/fluxSimulation/flight.root";
  QString groundFileName = dataPath + "/fluxSimulation/groundKiruna.root";
  if (QFile::exists(flightFileName))
    readKeys(flightFileName);
  if (QFile::exists(groundFileName))
    readKeys(groundFileName);
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
        TH2D* histogram = static_cast<TH2D*>(object);
        m_fluxes.insert(key, SimulationFlux(key, histogram).spectrum());
      }
    }
  }
  file.Close();
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

QVector<SimulationFluxKey::Location> SimulationFluxReader::locations() const
{
  QVector<SimulationFluxKey::Location> locations;
  foreach (SimulationFluxKey key, m_fluxes.keys())
    if (!locations.contains(key.location()))
      locations.append(key.location());
  return locations;
}

QVector<SimulationFluxKey::Acceptance> SimulationFluxReader::acceptances(SimulationFluxKey::Location location) const
{
  QVector<SimulationFluxKey::Acceptance> acceptances;
  foreach (SimulationFluxKey key, m_fluxes.keys())
    if (location == key.location() && !acceptances.contains(key.acceptance()))
      acceptances.append(key.acceptance());
  qSort(acceptances);
  return acceptances;
}

QVector<SimulationFluxKey::Source> SimulationFluxReader::sources(SimulationFluxKey::Location location) const
{
  QVector<SimulationFluxKey::Source> sources;
  foreach (SimulationFluxKey key, m_fluxes.keys())
    if (location == key.location() && !sources.contains(key.source()))
      sources.append(key.source());
  qSort(sources);
  return sources;
}

QVector<Enums::Particle> SimulationFluxReader::particles(SimulationFluxKey::Location location) const
{
  QVector<Enums::Particle> particles;
  foreach (SimulationFluxKey key, m_fluxes.keys())
    if (location == key.location() && !particles.contains(key.particle()))
      particles.append(key.particle());
  return particles;
}

QVector<double> SimulationFluxReader::modulationParameters(SimulationFluxKey::Location location) const
{
  QVector<double> modulationParameters;
  foreach (SimulationFluxKey key, m_fluxes.keys())
    if (location == key.location() && !modulationParameters.contains(key.modulationParameter()))
      modulationParameters.append(key.modulationParameter());
  qSort(modulationParameters);
  return modulationParameters;
}
