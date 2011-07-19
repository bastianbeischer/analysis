#ifndef TimeOfFlight_hh
#define TimeOfFlight_hh

#include <QVector>

class Track;

class TimeOfFlight
{
  
public:
  TimeOfFlight();
  ~TimeOfFlight();
  
public:
  void reset();
  void calculateTimes(const Track*);

  double startTime() const {return m_startTime;}
  double stopTime() const {return m_stopTime;}
  double timeOfFlight() const {return m_stopTime - m_startTime;}

private:
  static double lastGoodTime(QVector<double>&);
  static double bestTime(QVector<double>&);

private:
  double m_startTime;
  double m_stopTime;

};

#endif /* TimeOfFlight_hh */
