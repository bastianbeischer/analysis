#ifndef SimpleEvent_hh
#define SimpleEvent_hh

#include <TObject.h>

#include "Hit.hh"

class SimpleEvent :
  public TObject
{
  
public:
  SimpleEvent();
  SimpleEvent(int id, int time);
  SimpleEvent(const SimpleEvent& other);
  const SimpleEvent& operator=(const SimpleEvent& right);
  ~SimpleEvent();

public:
  std::vector<Hit*>& hits()           {return m_hits;}
  int                eventId() const  {return m_eventId;}
  int                time()    const  {return m_time;}

  void               addHit(Hit* hit) {m_hits.push_back(hit);}

private:
  int               m_eventId;
  int               m_time;
  std::vector<Hit*> m_hits;

  ClassDef( SimpleEvent, 1 );
  
};

#endif /* SimpleEvent_hh */
