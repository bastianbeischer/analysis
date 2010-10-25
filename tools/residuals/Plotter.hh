#ifndef Plotter_hh
#define Plotter_hh

#include <QMap>

class DataChain;
class Layer;
class Track;
class TrackFinding;
class ResidualPlot;

class Plotter
{
  
public:
  Plotter();
  ~Plotter();
  
public:
  void addFiles(const char* listName);
  void process();
  void draw();

private:
  DataChain*                  m_chain;

  QMap<Layer*, ResidualPlot*> m_residualPlots;
  QMap<Layer*, Track*>        m_tracks;
  TrackFinding*               m_trackFinding;

};

#endif /* Plotter_hh */
