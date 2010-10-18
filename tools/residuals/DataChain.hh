#ifndef DataChain_hh
#define DataChain_hh

#include <vector>
#include <map>

class Hit;
class TChain;
class Event;
class Layer;
class Track;
class TrackFinding;

class DataChain
{
  
public:
  DataChain();
  ~DataChain();
  
public:
  void addFiles(const char* listName);
  void process();
  void draw();

private:
  std::vector<Hit*> applyCuts(std::vector<Hit*> hits) const;

private:
  TChain*                    m_chain;
  Event*                     m_event;

  std::map<double, Layer*>   m_layers;
  std::map<Layer*, Track*>   m_tracks;
  TrackFinding*              m_trackFinding;

};

#endif /* DataChain_hh */
