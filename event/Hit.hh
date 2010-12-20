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

  Hit(const Hit& other);

  virtual ~Hit();

public:
  void setSignalHeight(const int height) {m_signalHeight = height;}
  void setPosition(const TVector3 pos) {m_position = pos;}

public:
  ModuleType      type()            const {return m_type;}
  unsigned short  detId()           const {return m_detId;}
  int             signalHeight()    const {return m_signalHeight;}
  const TVector3& position()        const {return m_position;}
  const TVector3& counterPosition() const {return m_counterPosition;}
  double          angle()           const {return m_angle;}

public:
  double          resolutionEstimate() const;

  unsigned short  device()     const;
  unsigned short  connection() const;
  unsigned short  block()      const;
  unsigned short  channel()    const;

protected:
  void            calculateAngle();

protected:
  ModuleType     m_type;
  unsigned short m_detId;
  int            m_signalHeight;
  TVector3       m_position;
  TVector3       m_counterPosition;
  double         m_angle;

  ClassDef(Hit, 1);

};

#endif /* Hit_hh */
