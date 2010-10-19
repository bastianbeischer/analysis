#ifndef DataDescription_h
#define DataDescription_h

#include <TObject.h>

#include <string>
#include <vector>

class DataDescription : public TObject
{
  public:
    DataDescription();
    ~DataDescription();

    void setSoftwareVersionHash(const std::string& version) {m_softwareVersionHash = version;}
    const std::string& softwareVersionHash() const {return m_softwareVersionHash;}
    
    long numberOfRuns() const;
    int timeOfRun(int) const;
    
    long eventNumberInRunFile(long eventNumber) const;
    const std::string& runFileForEventNumber(long eventNumber) const;
    
  private:
    std::string m_comment;
    std::string m_softwareVersionHash; //SHA1 hash of the latest commit
    std::vector<long> m_eventNumberOffset;
    std::vector<std::string> m_runFileNames;

  ClassDef( DataDescription, 1 );
};

#endif
