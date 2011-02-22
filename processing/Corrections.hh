#ifndef Corrections_hh
#define Corrections_hh

#include <QFlags>
#include <QVector>

class QSettings;
class SimpleEvent;
class Hit;
class Track;

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
  void tofTimeOverThreshold(Hit*);
  void photonTravelTime(Track*); 

public:
  double trdScalingFactor(unsigned int);
  void setTrdScalingFactor(unsigned int, double);

private:
  QSettings* m_trdSettings;
  QSettings* m_tofSettings;
  Flags      m_flags;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Corrections::Flags);

#endif /* Corrections_hh */
