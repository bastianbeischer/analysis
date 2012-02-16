#ifndef DataChain_hh
#define DataChain_hh

#include <TChain.h>

#include <QMutex>
#include <QDateTime>

class SimpleEvent;
class DataDescription;

class DataChain
{

public:
  DataChain();
  DataChain(const char* listName);
  ~DataChain();

public:
  void clear();
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

  QDateTime time(int event);
  QDateTime startTime();
  QDateTime stopTime();

public:
  static QMutex    s_mutex;

private:
  TChain*          m_chain;
  SimpleEvent*     m_event;

  int              m_currentEntry;
  std::vector<unsigned int> m_runNumbers;
  std::vector<int> m_offsets;
  std::vector<const DataDescription*> m_descriptionBuffer;

};

#endif /* DataChain_hh */
