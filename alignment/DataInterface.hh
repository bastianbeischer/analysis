#ifndef DataInterface_hh
#define DataInterface_hh

class TrackSelection;

class DataInterface
{
  
public:
  DataInterface();
  ~DataInterface();
  
private:
  TrackSelection* m_trackSelection;
  
};

#endif /* DataInterface_hh */
