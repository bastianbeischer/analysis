#ifndef TrackSelection_hh
#define TrackSelection_hh

#include <QFlags>

class Track;

class TrackInformation
{
  
public:
  TrackInformation(const Track* track);
  ~TrackInformation();
  
  enum Flag {None=0x00, AllTrackerLayers=0x01, InsideMagnet=0x02, OutsideMagnet=0x04, HighPt=0x08, 
             MagnetCollision=0x10};

  Q_DECLARE_FLAGS(Flags, Flag);

public:
  void process();
  const Flags& flags() const {return m_flags;}

private:
  void checkAllTrackerLayers();
  void checkInsideMagnet();
  void checkOutsideMagnet();
  void checkHighPt();
  void checkMagnetCollision();

private:
  const Track* m_track;
  Flags m_flags;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(TrackInformation::Flags);

#endif /* TrackSelection_hh */
