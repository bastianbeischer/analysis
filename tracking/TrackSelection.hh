#ifndef TrackSelection_hh
#define TrackSelection_hh

class Track;

class TrackSelection
{
  
public:
  TrackSelection();
  ~TrackSelection();
  
  enum Flags {None=0,AllTrackerLayers=1,InsideMagnet=2, HighPt=4};

public:
  void processTrack(Track* track);
  Flags flags() const {return m_flags;}

private:
  void checkAllTrackerLayers(Track* track);
  void checkInsideMagnet(Track* track);
  void checkHighPt(Track* track);

private:
  Flags m_flags;

};

#endif /* TrackSelection_hh */
