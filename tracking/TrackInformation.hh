#ifndef TrackSelection_hh
#define TrackSelection_hh

#include <QFlags>

class Track;

class TrackInformation
{
  
public:
  TrackInformation(const Track* track);
  ~TrackInformation();
  
  enum Flag {None=0x0, AllTrackerLayers=0x1<<0, InsideMagnet=0x1<<1, OutsideMagnet=0x1<<2, HighPt=0x1<<3, 
             MagnetCollision=0x1<<4, Albedo=0x1<<5};

  Q_DECLARE_FLAGS(Flags, Flag);

public:
  void process();
  void reset() {m_flags = Flags(0);}
  const Flags& flags() const {return m_flags;}

private:
  void checkAllTrackerLayers();
  void checkInsideMagnet();
  void checkOutsideMagnet();
  void checkHighPt();
  void checkMagnetCollision();
  void checkAlbedo();

private:
  const Track* m_track;
  Flags m_flags;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(TrackInformation::Flags);

#endif /* TrackSelection_hh */
