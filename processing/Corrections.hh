#ifndef Corrections_hh
#define Corrections_hh

#include <QFlags>
#include <QVector>

class QSettings;
class Hit;

class Corrections
{
  
public:
  enum Flag {None = 0x0, Alignment = 0x1<<0, TimeShifts = 0x1<<1, TrdMopv = 0x1<<2, TofTimeOverThreshold = 0x1<<3};
  Q_DECLARE_FLAGS(Flags, Flag);

public:
  Corrections(Flags = ~Flags(0));
  ~Corrections();
  
public:
  void setFlags(const Flags& flags) {m_flags = flags;}
  Flags flags() const {return m_flags;}

public:
  void apply(QVector<Hit*>&);
  
private:
  void alignment(Hit*);
  void timeShift(Hit*);
  void trdMopv(Hit*);
  void tofTimeOverThreshold(Hit*);

public:
  double trdScalingFactor(unsigned int);
  void setTrdScalingFactor(unsigned int, double);

private:
  QSettings* m_trdSettings;
  Flags      m_flags;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Corrections::Flags);

#endif /* Corrections_hh */
