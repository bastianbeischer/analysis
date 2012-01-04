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
  double beta() const {return m_beta;}
  double timeOverThreshold() const {return m_timeOverThreshold;}
  double timeOfFlight() const {return m_stopTime - m_startTime;}
  bool good() const {return m_good;}

private:
  static double bestTime(const QVector<double>&);
  void sort(QVector<double>& t, QVector<double>& tot);

private:
  double m_startTime;
  double m_stopTime;
  double m_beta;
  double m_timeOverThreshold;
  bool m_good;
};

#endif /* TimeOfFlight_hh */
