#ifndef DataInterface_hh
#define DataInterface_hh

class TChain;
class TrackSelection;

class DataInterface
{
  
public:
  DataInterface();
  ~DataInterface();
  
public:
  void addFiles(const char* listName);

private:
  TChain*         m_chain;

  TrackSelection* m_trackSelection;
  
};

#endif /* DataInterface_hh */
