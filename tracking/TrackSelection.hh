#ifndef TrackSelection_hh
#define TrackSelection_hh

#include <QFlags>

class Track;

class TrackSelection
{
  
public:
  TrackSelection();
  ~TrackSelection();
  
  enum Flag {None=0, AllTrackerLayers=1, InsideMagnet=2, OutsideMagnet=4, HighPt=8, MagnetCollision=16};

  Q_DECLARE_FLAGS(Flags, Flag)

public:
  void processTrack(Track*);
  const Flags& flags() const {return m_flags;}

private:
  void checkAllTrackerLayers(Track*);
  void checkInsideMagnet(Track*);
  void checkOutsideMagnet(Track*);
  void checkHighPt(Track*);
  void checkMagnetCollision(Track*);

private:
  Flags m_flags;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(TrackSelection::Flags);

#endif /* TrackSelection_hh */
