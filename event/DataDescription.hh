#ifndef DataDescription_h
#define DataDescription_h

#include <TObject.h>

#include <string>
#include <vector>
#include <assert.h>

class DataDescription : public TObject
{
public:
  DataDescription();
  DataDescription(const DataDescription& other);
  ~DataDescription();

  static const std::string calculateSoftwareVersionHash();

  const std::string& comment() const {return m_comment;}
  void setComment(const std::string& comment) {m_comment = comment;}
  void setSoftwareVersionHash();
  void setSoftwareVersionHash(const std::string& version) {m_softwareVersionHash = version;}
  const std::string& softwareVersionHash() const {return m_softwareVersionHash;}
  long numberOfEventsInRunFile(int i) const;
  void addRunFile(const std::string& fileName, const std::string& softwareVersionHash, const int nEvents);
  const std::string& runFileName(int i) {assert(i < m_numberOfRuns); return m_runFileNames[i];}
  const std::vector<std::string>& runFileNames() {return m_runFileNames;}
  const std::string& runFileSoftwareVersionHash(int i) {assert(i < m_numberOfRuns); return m_runFileSoftwareVersionHash[i];}
    
  long numberOfRuns() const {return m_numberOfRuns;}
  int timeOfRun(int) const;
    
  long eventNumberInRunFile(long eventNumber) const;
  int runFileForEventNumber(long eventNumber) const;
  const std::string& runFileNameForEventNumber(long eventNumber) const;
    
private:
  std::string m_comment;
  std::string m_softwareVersionHash; //SHA1 hash of the latest commit
  std::vector<long> m_eventNumberOffset;
  std::vector<std::string> m_runFileNames;
  std::vector<std::string> m_runFileSoftwareVersionHash;
  int m_numberOfRuns;

  ClassDef( DataDescription, 1 );
};

#endif
