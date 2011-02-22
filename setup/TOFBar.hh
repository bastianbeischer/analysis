#ifndef TOFBar_hh
#define TOFBar_hh

#include <QVector>

#include "DetectorElement.hh"

class Cluster;

class TOFBar :
  public DetectorElement
{
  
public:
  explicit TOFBar(unsigned short detId, const Setup* setup);
  ~TOFBar();
public:
  QVector<Hit*> findClusters();
  void setTimeShifts(double ch0, double ch1, double ch2, double ch3);
  double timeShift(unsigned short channel);

private:
  QVector<double> m_timeShifts;

};

#endif /* TOFBar_hh */
