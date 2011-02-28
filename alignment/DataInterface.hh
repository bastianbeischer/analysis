#ifndef DataInterface_hh
#define DataInterface_hh

class AnalysisProcessor;
class AlignmentMatrix;
class DataChain;
class TrackFinding;
class Corrections;

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
  AnalysisProcessor* m_processor;

};

#endif /* DataInterface_hh */
