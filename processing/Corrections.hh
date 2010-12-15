#ifndef Corrections_hh
#define Corrections_hh

#include <QFlags>
#include <QVector>

class Hit;

class Corrections
{
  
public:
  enum Flag {None = 0x0, Alignment = 0x1, TimeShifts = 0x2, TrdMopv = 0x4, TofTimeOverThreshold = 0x8};
  Q_DECLARE_FLAGS(Flags, Flag);

public:
  Corrections();
  ~Corrections();
  
public:
  void apply(QVector<Hit*>&, Flags = ~Flags(0));
  
private:
  void alignment(Hit*);
  void timeShift(Hit*);
  void trdMopv(Hit*);
  void tofTimeOverThreshold(Hit*);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Corrections::Flags);

#endif /* Corrections_hh */
