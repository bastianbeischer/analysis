#ifndef SimpleEvent_hh
#define SimpleEvent_hh

#include <TObject.h>

#include "Hit.hh"

class SimpleEvent :
  public TObject
{

public:
  enum ContentType {None = -1, RawData, Clusters};

public:
  SimpleEvent();
  SimpleEvent(int id, int time, ContentType type);
  SimpleEvent(const SimpleEvent& other);
  ~SimpleEvent();

public:
  std::vector<Hit*>& hits()              {return m_hits;}
  int                eventId() const     {return m_eventId;}
  int                time()    const     {return m_time;}
  ContentType        contentType() const {return m_contentType;}

  void               addHit(Hit* hit) {m_hits.push_back(hit);}

private:
  int               m_eventId;
  int               m_time;
  ContentType       m_contentType;
  std::vector<Hit*> m_hits;

  ClassDef( SimpleEvent, 1 );
  
};

#endif /* SimpleEvent_hh */
