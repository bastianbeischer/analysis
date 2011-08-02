#ifndef SimulationFluxReader_hh
#define SimulationFluxReader_hh

#include "Particle.hh"
#include "SimulationFlux.hh"
#include "SimulationFluxKey.hh"

#include <TH1D.h>
#include <TFile.h>

#include <QList>
#include <QString>
#include <QMap>
#include <QMutex>

class SimulationFluxReader {

public:
  enum Location {
    NotSet,
    GroundEsrange,
    Flight
  };
  enum Acceptance {
    None,
    InsideMagnet,
    Tof,
    TwoPi
  };

  SimulationFluxReader(Location location);
	~SimulationFluxReader();

  static QList<Location> allLocations();
  static QString locationName(Location location);
  static Location location(QString locationName);

  static QList<Acceptance> allAcceptances();
  static QString acceptanceName(Acceptance acceptance);
  static Acceptance acceptance(QString acceptanceName);

  void setAcceptance(Acceptance acceptance) {s_acceptance = acceptance;}

  QList<double> phis() const {return s_phis;}

  TH1D* spectrum(SimulationFluxKey key);
  TH1D* spectrum(QList<SimulationFluxKey> keys);
  static void reset();

private:
  static QString s_folder;

  static Location s_location;
  static Acceptance s_acceptance;
  static QMap<SimulationFluxKey, TH1D*> s_fluxes;
  static QList<double> s_phis;
  static QList<Acceptance> s_acceptances;

  static bool checkForFile(QString folder, Location location);
  static QString filename(QString folder, SimulationFluxReader::Location location);
  void setPhis();
  void setAcceptances();

  static bool s_constructed;
  static void construct();
  static QMap<Location, QString> s_locationNames;
  static QMap<Acceptance, QString> s_acceptanceNames;
  QString assembleHistogramName(Acceptance acceptance, Location location, SimulationFluxKey key);
  QString simulationLocation(Location location);
  QString simulationAcceptance(Acceptance acceptance);
  QMutex m_mutex;
};


#endif
