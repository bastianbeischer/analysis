#ifndef TrackSelection_hh
#define TrackSelection_hh

class Track;

class TrackSelection
{
  
public:
  TrackSelection();
  ~TrackSelection();
  
  bool passes(Track* track);

private:
  
  
};

#endif /* TrackSelection_hh */
