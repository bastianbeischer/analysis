#ifndef TrackSelection_hh
#define TrackSelection_hh

class Track;

class TrackSelection
{
  
public:
  TrackSelection();
  ~TrackSelection();
  
  enum Flags {None=0,AllTrackerLayers=1, InsideMagnet=2, OutsideMagnet=4, HighPt=8, MagnetCollision=16};

public:
  void processTrack(Track*);
  Flags flags() const {return m_flags;}

private:
  void checkAllTrackerLayers(Track*);
  void checkInsideMagnet(Track*);
  void checkOutsideMagnet(Track*);
  void checkHighPt(Track*);
  void checkMagnetCollision(Track*);

private:
  Flags m_flags;

};

#endif /* TrackSelection_hh */
