#ifndef DataInterface_hh
#define DataInterface_hh

class AlignmentMatrix;
class DataChain;
class TrackSelection;
class TrackFinding;

class DataInterface
{
  
public:
  DataInterface();
  ~DataInterface();
  
public:
  void addFiles(const char* listName);
  void process(AlignmentMatrix* matrix);

private:
  DataChain*      m_chain;

  TrackSelection* m_trackSelection;
  TrackFinding*   m_trackFinding;
  
};

#endif /* DataInterface_hh */
