#include "SimulationFluxReader.hh"

#include <Constants.hh>

#include <QFile>
#include <QDebug>
#include <QStringList>

#include <TFile.h>
#include <TObject.h>
#include <TROOT.h>
#include <TKey.h>

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

bool SimulationFluxReader::s_constructed = false;
QMap<SimulationFluxReader::Location, QString> SimulationFluxReader::s_locationNames;
QMap<SimulationFluxReader::Acceptance, QString> SimulationFluxReader::s_acceptanceNames;

SimulationFluxReader::Location SimulationFluxReader::s_location = SimulationFluxReader::NotSet;
SimulationFluxReader::Acceptance SimulationFluxReader::s_acceptance = SimulationFluxReader::None;
QMap<SimulationFluxKey, TH1D*> SimulationFluxReader::s_fluxes;
QList<double> SimulationFluxReader::s_phis;
QList<SimulationFluxReader::Acceptance> SimulationFluxReader::s_acceptances;
QString SimulationFluxReader::s_folder;

void SimulationFluxReader::reset()
{
  SimulationFluxReader::s_constructed = false;
  s_locationNames.clear();
  s_acceptanceNames.clear();

  SimulationFluxReader::s_location = SimulationFluxReader::NotSet;
  SimulationFluxReader::s_acceptance = SimulationFluxReader::None;
  foreach(TH1D* flux, s_fluxes) {
    if (flux) {
      delete flux;
    }
  }
  s_fluxes.clear();
  s_phis.clear();
  s_acceptances.clear();
}

SimulationFluxReader::SimulationFluxReader(Location location)
{
  s_acceptance = InsideMagnet;
  m_mutex.lock();

  construct();
  if (s_location != location) {
    s_location = location;

    foreach(TH1D* flux, s_fluxes) {
      if (flux) {
        delete flux;
      }
    }

    s_fluxes.clear();
    s_phis.clear();


    setPhis();

    QString name = filename(s_folder, s_location);

    TFile* file = 0;
    if (QFile::exists(name)) {
      qDebug() << "Opening file"<< name;
      file = new TFile(qPrintable(name));
    } else {
      qFatal("ERROR: Could not find file %s", qPrintable(name));
    }

    gROOT->cd();

    foreach(double phi, s_phis) {
      qDebug()<<"reading phi"<<phi;
      foreach(SimulationFluxKey::Source source, SimulationFluxKey::allSources()) {
        for(int isAlbedo = 0; isAlbedo < 2; isAlbedo++) {
          foreach(Particle::Type type, SimulationFluxKey::allParticles()) {
            SimulationFluxKey currentKey(phi, type, isAlbedo, source);
            QString hName = assembleHistogramName(InsideMagnet, location, currentKey);
            TH2D* histogram = ( (TH2D*)(((TH2D*)file->Get(qPrintable(hName)))->Clone()) );
            SimulationFlux flux(currentKey, histogram);
            TH1D* h1 = flux.spectrum();
            s_fluxes.insert(currentKey, h1);
          }
        }
      }
    }

    file->Close();
    delete file;
    file = 0;
    qDebug("Closed File!");
  }
  m_mutex.unlock();
}

SimulationFluxReader::~SimulationFluxReader()
{
  foreach(TH1D* flux, s_fluxes) {
    if (flux) {
      delete flux;
    }
  }
  s_fluxes.clear();
}

void SimulationFluxReader::construct()
{
  if (!s_constructed) {

    char* env = getenv("PERDAIXDATA_PATH");
    s_folder = env;
    s_folder.append("/fluxSimulation");

    if (checkForFile(s_folder, Flight)) {
      s_locationNames.insert(Flight, "Flight");
    }
    if (checkForFile(s_folder, GroundEsrange)) {
      s_locationNames.insert(GroundEsrange, "Ground Esrange");
    }

    s_acceptanceNames.insert(InsideMagnet, "Inside Magnet ("+QString::number(Constants::geometricAcceptance*pow(100, 2))+" cm^2sr)");
    s_acceptanceNames.insert(Tof, "Tof ("+QString::number(Constants::geometricAcceptanceTof*pow(100, 2))+" cm^2sr)");
    s_acceptanceNames.insert(TwoPi, "Two Pi");
    s_constructed = true;
  }
}

TH1D* SimulationFluxReader::spectrum(SimulationFluxKey key)
{
  TH1D* flux = new TH1D(*s_fluxes[key]);
  if (!flux->GetSumw2())
    flux->Sumw2();
  return flux;
}

TH1D* SimulationFluxReader::spectrum(QList<SimulationFluxKey> keys)
{
  TH1D* flux =  new TH1D(*s_fluxes[keys.at(0)]);
  if (!flux->GetSumw2())
    flux->Sumw2();
  QString title = "MC";
  title.append(" "+Particle(keys.at(0).type()).name());
  for (int i = 1; i < keys.size(); i++) {
    SimulationFluxKey key = keys[i];
    flux->Add(s_fluxes[keys.at(i)]);
    title.append(" "+Particle(keys.at(i).type()).name());
  }
  title.append(" sum");
  flux->SetTitle(qPrintable(title));
  flux->SetLineColor(kBlue-2);
  flux->SetMarkerColor(kBlue-2);
  return flux;
}

void SimulationFluxReader::setAcceptances()
{
  QList<Acceptance> acceptances;
  QString name = filename(s_folder, s_location);
  TFile* file = 0;
  if (QFile::exists(name)) {
		qDebug() << "Opening file"<< name;
		file = new TFile(qPrintable(name));
	} else {
    qFatal("ERROR: Could not find file %s", qPrintable(name));
	}

  for (int i = 0; i < file->GetListOfKeys()->GetEntries(); i++) {
    TKey* key =  (TKey*)file->GetListOfKeys()->At(i);
    TObject* object = key->ReadObj();
    QString className = object->ClassName();
    if (className.contains("TH2D")) {
      QString objectName = object->GetName();
      if (!objectName.contains("rigidity")) {
        continue;
      }
      QString currentName = objectName.split(simulationLocation(s_location)+"_").at(1).split("rigidity").at(0);

      Acceptance a;
      foreach(Acceptance acc, allAcceptances()) {
        QString accName = simulationAcceptance(acc);
        if (accName == currentName) {
          a = acc;
        }
      }

      if (!acceptances.contains(a)) {
        acceptances.push_back(a);
        int nPhis = 1;
        if (s_phis.size() != 0) {
          nPhis = s_phis.size();
        }
        i += SimulationFluxKey::allSources().size() * SimulationFluxKey::allParticles().size()*nPhis;

      }
    }
  }

  file->Close();
	delete file;
	file = 0;

  s_acceptances = acceptances;
}

bool SimulationFluxReader::checkForFile(QString folder, Location location)
{
  QString name = filename(folder, location);
  if (QFile::exists(name)) {
		qDebug("Found file %s", qPrintable(name));
    return true;
	} else {
    qDebug("Could not find file %s", qPrintable(name));
    return false;
	}

}

void SimulationFluxReader::setPhis()
{
  QList<double> phis;
  QString name = filename(s_folder, s_location);
  TFile* file = 0;
  if (QFile::exists(name)) {
		qDebug() << "Opening file"<< name;
		file = new TFile(qPrintable(name));
	} else {
    qFatal("ERROR: Could not find file %s", qPrintable(name));
	}

  for (int i = 0; i < file->GetListOfKeys()->GetEntries(); i++) {
    TKey* key =  (TKey*)file->GetListOfKeys()->At(i);
    TObject* object = key->ReadObj();
    QString className = object->ClassName();
    if (className.contains("TH2D")) {
      QString objectName = object->GetName();
      if (!objectName.contains("rigidity")) {
        continue;
      }
      double phi = objectName.split("_phi_").at(1).split("_MV_").at(0).toDouble();
      if (!phis.contains(phi)) {
        phis.push_back(phi);
        i += SimulationFluxKey::allSources().size() * SimulationFluxKey::allParticles().size();
      }
    }
  }

  file->Close();
	delete file;
	file = 0;

  qSort(phis);

  s_phis = phis;
}

QString SimulationFluxReader::filename(QString folder, SimulationFluxReader::Location location)
{
  QString filename = folder+"/";
  switch (location) {
    case SimulationFluxReader::GroundEsrange:
      return filename.append("corsikaAnalysis_2011-07-06.root");
    case SimulationFluxReader::Flight:
      return filename.append("planetocosmicsAnalysis_2011-07-06.root");
    default:
      qDebug("filename for this location not specified");
      return "";
  }
}

QString SimulationFluxReader::simulationLocation(Location location)
{
  switch (location) {
    case GroundEsrange:
      return "corsika";
    case Flight:
      return "planetocosmics";
    default:
      qDebug("locationName not specified");
      return "";
  }
}

QString SimulationFluxReader::simulationAcceptance(Acceptance acceptance)
{
  switch (acceptance) {
    case InsideMagnet:
      return "perdaix";
    case Tof:
      return "perdaixTof";
    case TwoPi:
      return "";
    default:
      qDebug("acceptanceName not specified");
      return "";
  }
}

QString SimulationFluxReader::assembleHistogramName(Acceptance acceptance, Location location, SimulationFluxKey key)
{
  const QString xAxisName = simulationAcceptance(acceptance)+"rigidity";
  return simulationLocation(location)+"_"+xAxisName+"_"+key.internalName()+"_phi_"+QString::number(key.phi())+"_MV_"+key.sourceName();
}

QString SimulationFluxReader::locationName(SimulationFluxReader::Location location)
{
  construct();
  return s_locationNames[location];
}

QList<SimulationFluxReader::Location> SimulationFluxReader::allLocations()
{
  construct();
  QList<SimulationFluxReader::Location> locs = s_locationNames.keys();
  if (locs.size() > 1) {
    locs.swap(0, 1);
  }
  return locs;
}

SimulationFluxReader::Location SimulationFluxReader::location(QString locationName)
{
  construct();
  return s_locationNames.key(locationName);
}

QString SimulationFluxReader::acceptanceName(SimulationFluxReader::Acceptance acceptance)
{
  construct();
  return s_acceptanceNames[acceptance];
}

QList<SimulationFluxReader::Acceptance> SimulationFluxReader::allAcceptances()
{
  construct();
  return s_acceptanceNames.keys();
}

SimulationFluxReader::Acceptance SimulationFluxReader::acceptance(QString acceptanceName)
{
  construct();
  return s_acceptanceNames.key(acceptanceName);
}
