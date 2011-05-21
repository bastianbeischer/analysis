#ifndef Corrections_hh
#define Corrections_hh

#include "Constants.hh"

#include <QFlags>
#include <QVector>
#include <QString>
#include <QVariant>
#include <QList>
#include <QMap>
#include <QList>
#include <QPair>

class QSettings;
class SimpleEvent;
class Hit;
class Particle;
class SimpleEvent;

class Corrections
{
  
public:
  enum Flag {None = 0x0, Alignment = 0x1<<0, TimeShifts = 0x1<<1, TrdMopv = 0x1<<2, TrdPressure = 0x1<<3, TofTimeOverThreshold = 0x1<<4,
             MultipleScattering = 0x1<<5, PhotonTravelTime = 0x1<<6};
  Q_DECLARE_FLAGS(Flags, Flag);

public:
  Corrections(Flags = ~Flags(0));
  ~Corrections();
  
public:
  void setFlags(const Flags& flags) {m_flags = flags;}
  Flags flags() const {return m_flags;}

public:
  void preFitCorrections(SimpleEvent*);
  void postFitCorrections(Particle*);

  static const int nPhotonTravelTimeParameters = 3;
  static const int nPhotonTravelTimeDifferenceParameters = 6;
  static double photonTravelTime(double bending, double nonBending, double* p);
  static double photonTravelTimeDifference(double bending, double nonBending, double* p);
  static const int nTotScalingParameters = 2;

private:
  void alignment(Hit*);
  void timeShift(Hit*);
  void trdMopv(Hit*);
  void trdPressure(Hit*, SimpleEvent* event);
  void tofTot(Hit* hit, SimpleEvent* event);
  void multipleScattering(Particle*);
  void photonTravelTime(Particle*); 

public:
  double trdScalingFactor(unsigned int);
  void setTrdScalingFactor(unsigned int, double);
  double trdPressureDependendFactor(double P);
  void setTrdPressureDependendFactor(QPair<double,double> P0, double dM_dP);
  void getTrdPressureDependendFactor(QPair<double,double>& P0, double& dM_dP);
  void setTotScaling(const unsigned int tofId, const QList<QVariant> param);
  
private:
  QString m_tofTotScalingPrefix;
  double m_totScalings[Constants::nTofChannels][nTotScalingParameters];
  void loadTotScaling();
  double totScalingFactor(const unsigned int tofId, const double temperature);
  unsigned int tofChannel(unsigned int id);
  
private:
  QSettings* m_trdSettings;
  QSettings* m_tofSettings;
  Flags      m_flags;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Corrections::Flags);

#endif /* Corrections_hh */
