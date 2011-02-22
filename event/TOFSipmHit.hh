#ifndef TOFSipmHit_hh
#define TOFSipmHit_hh

#include "Hit.hh"

#include <vector>
#include <stdint.h>

class TOFSipmHit : public Hit {
public:
  enum EdgeType {LeadingEdge, TrailingEdge};

public:
  TOFSipmHit();
  TOFSipmHit(unsigned short detId, TVector3 position, TVector3 counterPosition);
  TOFSipmHit(const TOFSipmHit& other);

  virtual ~TOFSipmHit();

  void setPhotonTravelTime(double);
  void applyTimeShift(double);
  void processTDCHits();

  double startTime() const {return m_startTime;}
  double photonTravelTime() const {return m_photonTravelTime;}
  double timeOverThreshold() const {return m_timeOverThreshold;}
  int numberOfLevelChanges() const {return m_levelChanges.size();}
  int channel() const;

  double time(int) const;
  EdgeType edgeType(int) const;
  int spillNumber(int) const;
  void setLevelChange(int, uint32_t);
  
  void addLevelChange(uint32_t);
  
  static int channelFromData(uint32_t);

  void dump();
  
  void setTimeOverThreshold(double timeOverThreshold);
  
pri  tic bool earlierThan(uint32_t, uint32_t);

  int m_channel;
  double m_startTime;
  double m_photonTravelTime; //! TODO: remove this comment line next time the data is processed!
  double m_timeOverThreshold;

  std::vector<uint32_t> m_levelChanges;

  ClassDef(TOFSipmHit, 1);
};

#endif
