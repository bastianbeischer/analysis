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
  bool passes(Track* track, Flags flags);

private:
  bool checkAllTrackerLayers(Track* track);
  bool checkInsideMagnet(Track* track);
  bool checkHighPt(Track* track);

};

#endif /* TrackSelection_hh */
