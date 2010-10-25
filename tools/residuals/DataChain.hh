#ifndef DataChain_hh
#define DataChain_hh

#include <QMap>

class Hit;
class TChain;
class SimpleEvent;
class Layer;
class Track;
class TrackFinding;
class ResidualPlot;

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
  TChain*                     m_chain;
  SimpleEvent*                m_event;

  QMap<Layer*, ResidualPlot*> m_residualPlots;
  QMap<Layer*, Track*>        m_tracks;
  TrackFinding*               m_trackFinding;

};

#endif /* DataChain_hh */
