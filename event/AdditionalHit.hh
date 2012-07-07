#ifndef AdditionalHit_hh
#define AdditionalHit_hh

#include <TObject.h>

class AdditionalHit : public TObject
{
public:
  enum Type {Undefined, PMT, ECAL, ExternalTracker};
  AdditionalHit();
  AdditionalHit(Type type, unsigned short detId, double signalHeight);
  AdditionalHit(const AdditionalHit& other);
  virtual ~AdditionalHit();

  Type type() const {return m_type;}
  unsigned short detId() const {return m_detId;}
  double signalHeight() const {return m_signalHeight;}
protected:
  Type m_type;
  unsigned short m_detId;
  double m_signalHeight;

  ClassDef(AdditionalHit, 1);
};

#endif /* AdditionalHit_hh */
