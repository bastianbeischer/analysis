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

  void setComment(const std::string& comment) {m_comment = comment;}
  void setSoftwareVersionHash();
  void setSoftwareVersionHash(const std::string& version) {m_softwareVersionHash = version;}
  void addRunFile(const std::string& fileName, const std::string& softwareVersionHash, const int nEvents);

  const std::string& comment() const {return m_comment;}
  const std::string& softwareVersionHash() const {return m_softwareVersionHash;}
  long numberOfRuns() const {return m_numberOfRuns;}
  const std::string& runFileName(int i = 0) const {assert(i < m_numberOfRuns); return m_runFileNames[i];}
  const std::string& runFileSoftwareVersionHash(int i = 0) const {assert(i < m_numberOfRuns); return m_runFileSoftwareVersionHash[i];}

  long numberOfEventsInRunFile(int i = 0) const;
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
