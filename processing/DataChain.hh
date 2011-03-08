#ifndef DataChain_hh
#define DataChain_hh

#include <TChain.h>

#include <QMutex>

class SimpleEvent;
class DataDescription;

class DataChain
{
  
public:
  DataChain();
  DataChain(const char* listName);
  ~DataChain();
  
public:
  void setFileList(const char* listName);
  void addFileList(const char* listName);
  void addRootFile(const char* fileName);

public:
  unsigned int nEntries() const {return m_chain->GetEntries();}

public:
  SimpleEvent* event(unsigned int i);
  SimpleEvent* nextEvent();

  const TFile* currentFile() const {return (const TFile*) m_chain->GetFile();}
  const TTree* currentTree() const {return (const TTree*) m_chain->GetTree();}
  const DataDescription* currentDescription() const;

  int entry() const {return m_currentEntry;}
  int entryInFile() const;
  const std::string& rawFileNameForEvent() const;

private:
  void init();

public:
  static QMutex    s_mutex;

private:
  TChain*          m_chain;
  SimpleEvent*     m_event;

  int              m_currentEntry;
  std::vector<int> m_offsets;

};

#endif /* DataChain_hh */
