#ifndef Corrections_hh
#define Corrections_hh

#include <QFlags>
#include <QVector>

class Hit;

class Corrections
{
  
public:
  enum CorrectionFlag {None = 0x0, Alignment = 0x1, TimeShifts = 0x2, TrdMopv = 0x4, TofTimeOverThreshold = 0x8};
  Q_DECLARE_FLAGS(CorrectionFlags, CorrectionFlag);

public:
  Corrections();
  ~Corrections();
  
public:
  static void apply(QVector<Hit*>&, CorrectionFlags = ~CorrectionFlags(0));
  
private:
  static void alignment(Hit*);
  static void timeShift(Hit*);
  static void trdMopv(Hit*);
  static void tofTimeOverThreshold(Hit*);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Corrections::CorrectionFlags);

#endif /* Corrections_hh */
