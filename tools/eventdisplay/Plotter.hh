#ifndef Plotter_hh
#define Plotter_hh

#include <QMap>

class DataChain;
class TrackFinding;

class Plotter
{
  
public:
  Plotter();
  ~Plotter();
  
public:
  void addFiles(const char* listName);
  void process();

private:
  DataChain*                  m_chain;

  TrackFinding*               m_trackFinding;

};

#endif /* Plotter_hh */
