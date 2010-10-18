#ifndef Event_hh
#define Event_hh

#include <TObject.h>

#include "Hit.hh"

class Event :
  public TObject
{
  
public:
  Event();
  Event(int id, int time);
  ~Event();

public:
  std::vector<Hit*>& hits()           {return m_hits;}
  int                eventId() const  {return m_eventId;}
  int                time()    const  {return m_time;}

  void               addHit(Hit* hit) {m_hits.push_back(hit);}

private:
  int               m_eventId;
  int               m_time;
  std::vector<Hit*> m_hits;

  ClassDef( Event, 1 );
  
};

#endif /* Event_hh */
