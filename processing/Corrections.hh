#ifndef Corrections_hh
#define Corrections_hh

#include "Constants.hh"
#include "Enums.hh"

#include <QFlags>
#include <QVector>
#include <QString>
#include <QVariant>
#include <QList>
#include <QMap>
#include <QList>
#include <QPair>
#include <TSpline.h>

class QSettings;
class SimpleEvent;
class Hit;
class Particle;
class SimpleEvent;

class Corrections
{
public:
  Corrections(Enums::Corrections = Enums::NoCorrection);
  ~Corrections();
  
public:
  void setFlags(const Enums::Corrections& flags) {m_flags = flags;}
  Enums::Corrections flags() const {return m_flags;}

public:
  void preFitCorrections(SimpleEvent*);
  void postFitCorrections(Particle*);
  void postTOFCorrections(Particle*);

  static const int nPhotonTravelTimeParameters = 3;
  static const int nPhotonTravelTimeDifferenceParameters = 6;
  static double photonTravelTime(double bending, double nonBending, double* p);
  static double photonTravelTimeDifference(double bending, double nonBending, double* p);
  static const int nTotScalingParameters = 2;

private:
  void alignment(Hit*);
  void timeShift(Hit*);
  void trdMopv(Hit*);
  void trdTime(Hit*, SimpleEvent* event);
  void trdPressure(Hit*, SimpleEvent* event);
  void trdTemperature(Hit*, SimpleEvent* event);
  void tofTot(Hit* hit, SimpleEvent* event);
  void multipleScattering(Particle*);
  void photonTravelTime(Particle*); 

public:
  double trdScalingFactor(unsigned int);
  void setTrdScalingFactor(unsigned int, double);
  double trdPressureDependendFactor(double P);
  void setTrdPressureDependendFactor(QPair<double,double> P0, double dM_dP);
  void trdPressureDependendFactor(QPair<double,double>& P0, double& dM_dP);
  double trdTemperatureDependendFactor(double T);
  void setTrdTemperatureDependendFactor(QPair<double,double> T0, double dM_dT);
  void trdTemperatureDependendFactor(QPair<double,double>& T0, double& dM_dT);
  void addTrdTimeDependendFactor(double time, double factor);
  void removeTrdTimeDependendFactors(double startTime, double endTime);
  double trdTimeDependendFactor(double time);
  void setTotScaling(const unsigned int tofId, const QList<QVariant> param);
  
private:
  QString m_tofTotScalingPrefix;
  double m_totScalings[Constants::nTofChannels][nTotScalingParameters];
  void loadTotScaling();
  double totScalingFactor(const unsigned int tofId, const double temperature);
  unsigned int tofChannel(unsigned int id);
  void readTRDTimeDependendCorrections();
  void writeTRDTimeDependendCorrections();

public:
  TSpline3* trdTimeSpline() const;
  QMap<double, double> trdTimeFactors() const {return m_TRDMapTime;}
  
private:
  QSettings* m_trdSettings;
  QSettings* m_tofSettings;
  Enums::Corrections m_flags;
  TSpline3* m_TRDSplineTime;
  QMap<double, double> m_TRDMapTime;
};

#endif /* Corrections_hh */
