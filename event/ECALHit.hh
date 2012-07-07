#ifndef ECALHit_hh
#define ECALHit_hh

#include <TObject.h>

class ECALHit : public TObject
{
public:
  ECALHit();
  ECALHit(unsigned short detId, double signalHeight);
  ECALHit(const ECALHit& other);
  virtual ~ECALHit();

  unsigned short detId() const {return m_detId;}
  double signalHeight() const {return m_signalHeight;}
protected:
  unsigned short m_detId;
  double m_signalHeight;

  ClassDef(ECALHit, 1);
};

#endif /* ECALHit_hh */
