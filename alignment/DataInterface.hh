#ifndef DataInterface_hh
#define DataInterface_hh

class DataChain;
class TrackSelection;

class DataInterface
{
  
public:
  DataInterface();
  ~DataInterface();
  
public:
  void addFiles(const char* listName);
  void addSuitableTracks();

private:
  DataChain*      m_chain;

  TrackSelection* m_trackSelection;
  
};

#endif /* DataInterface_hh */
