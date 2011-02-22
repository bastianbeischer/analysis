#ifndef Corrections_hh
#define Corrections_hh

#include <QFlags>
#include <QVector>
#include <QString>
#include <QVariant>
#include <QMap>

#include <TGraph.h>
#include <TF1.h>

class QSettings;
class SimpleEvent;
class Hit;
class Track;
class SimpleEvent;

class Corrections
{
  
public:
  enum Flag {None = 0x0, Alignment = 0x1<<0, TimeShifts = 0x1<<1, TrdMopv = 0x1<<2, TofTimeOverThreshold = 0x1<<3, PhotonTravelTime = 0x1<<4};
  Q_DECLARE_FLAGS(Flags, Flag);

public:
  Corrections(Flags = ~Flags(0));
  ~Corrections();
  
public:
  void setFlags(const Flags& flags) {m_flags = flags;}
  Flags flags() const {return m_flags;}

public:
  void preFitCorrections(SimpleEvent*);
  void postFitCorrections(Track*);

  static const int numberOfPhotonTravelTimeParameters = 3;
  static const int numberOfPhotonTravelTimeDifferenceParameters = 6;
  static double photonTravelTime(double bending, double nonBending, double* p);
  static double photonTravelTimeDifference(double bending, double nonBending, double* p);

private:
  void alignment(Hit*);
  void timeShift(Hit*);
  void trdMopv(Hit*);
  void tofTimeOverThreshold(Hit* hit, SimpleEvent* event);
  void photonTravelTime(Track*); 

public:
  double trdScalingFactor(unsigned int);
  void setTrdScalingFactor(unsigned int, double);
  void setTimeOverThresholdScaling(const unsigned int tofChannel, const QMap<QString, QVariant> temperatureMap);

public:
  static QMap<unsigned int, TGraph> timeOverThresholdScalingGraphs;
  static QMap<unsigned int, TF1> timeOverThresholdScalingFits;
  static bool totGraphsLoaded;
  static QMap<unsigned int, double> minTofTemps;
  static QMap<unsigned int , double> maxTofTemps;
  
private:
  QString m_tofTimeOverThresholdPrefix;
  void loadTimeOverThresholdScaling();
  double timeOverThresholdScalingFactor(const unsigned int tofChannel, const double temperature);
  
private:
  QSettings* m_trdSettings;
  QSettings* m_tofSettings;
  Flags      m_flags;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Corrections::Flags);

#endif /* Corrections_hh */
