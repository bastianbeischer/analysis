#ifndef Hit_hh
#define Hit_hh

#include <TObject.h>
#include <TVector3.h>

class Hit :
  public TObject
{

public:
  enum ModuleType {none=-1, tracker, trd, tof};

public:
  Hit();

  Hit(ModuleType typ,
      unsigned short detId,
      int signalHeight,
      TVector3 position,
      TVector3 counterPosition);

  ~Hit();

public:
  ModuleType     type()            const {return m_type;}
  unsigned short detId()           const {return m_detId;}
  int            signalHeight()    const {return m_signalHeight;}
  TVector3       position()        const {return m_position;}
  TVector3       counterPosition() const {return m_counterPosition;}

public:
  double         angle()              const;
  double         resolutionEstimate() const;

  unsigned short device()     const;
  unsigned short connection() const;
  unsigned short block()      const;
  unsigned short channel()    const;

protected:
  ModuleType     m_type;
  unsigned short m_detId;
  int            m_signalHeight;
  TVector3       m_position;
  TVector3       m_counterPosition;

  ClassDef(Hit, 1);

};

#endif /* Hit_hh */
