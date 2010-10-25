#ifndef DataInterface_hh
#define DataInterface_hh

class TChain;
class TrackSelection;
class SimpleEvent;

class DataInterface
{
  
public:
  DataInterface();
  ~DataInterface();
  
public:
  void addFiles(const char* listName);
  void addSuitableTracks();

private:
  TChain*         m_chain;

  SimpleEvent*    m_event;
  TrackSelection* m_trackSelection;
  
};

#endif /* DataInterface_hh */
