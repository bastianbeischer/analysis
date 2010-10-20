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
    ~DataDescription();

    const std::string& comment() const {return m_comment;}
    void setComment(const std::string& comment) {m_comment = comment;}
    void setSoftwareVersionHash(const std::string& version) {m_softwareVersionHash = version;}
    const std::string& softwareVersionHash() const {return m_softwareVersionHash;}
    long numberOfEventsInRunFile(int i) const;
    void addRunFile(const std::string& fileName);
    const std::string& runFileName(int i) {assert(i < m_numberOfRuns); return m_runFileNames[i];}
    const std::vector<std::string>& runFileNames() {return m_runFileNames;}
    
    long numberOfRuns() const {return m_numberOfRuns;}
    int timeOfRun(int) const;
    
    long eventNumberInRunFile(long eventNumber) const;
    const std::string& runFileForEventNumber(long eventNumber) const;
    
  private:
    std::string m_comment;
    std::string m_softwareVersionHash; //SHA1 hash of the latest commit
    std::vector<long> m_eventNumberOffset;
    std::vector<std::string> m_runFileNames;
    int m_numberOfRuns;

  ClassDef( DataDescription, 1 );
};

#endif
