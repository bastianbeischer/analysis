#ifndef TOFBar_hh
#define TOFBar_hh

#include <QVector>

#include "DetectorElement.hh"

class Cluster;

class TOFBar :
  public DetectorElement
{
  
public:
  TOFBar();
  TOFBar(unsigned short detId);
  ~TOFBar();
public:
  QVector<Hit*> findClusters();
  void setTimeShifts(double ch0, double ch1, double ch2, double ch3);
  const QVector<double>& timeShifts();
private:
  QVector<double> m_timeShifts;
};

#endif /* TOFBar_hh */
