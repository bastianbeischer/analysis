#ifndef DataChain_hh
#define DataChain_hh

#include <TChain.h>

class SimpleEvent;

class DataChain
{
  
public:
  DataChain();
  DataChain(const char* listName);
  ~DataChain();
  
public:
  void addFiles(const char* listName);

public:
  unsigned int nEntries() const {return m_chain->GetEntries();}

public:
  SimpleEvent* event(unsigned int i);
  SimpleEvent* nextEvent();

private:
  void         init();

private:
  TChain*       m_chain;
  SimpleEvent*  m_event;

  int           m_currentEntry;

};

#endif /* DataChain_hh */
