#ifndef MeasurementTimeCalculation_hh
#define MeasurementTimeCalculation_hh

#include "Settings.hh"

#include <QVector>

class TH1D;
class SimpleEvent;

class MeasurementTimeCalculation {
public:
 MeasurementTimeCalculation(int numberOfThreads);
  MeasurementTimeCalculation(TH1D* histogram);
  virtual ~MeasurementTimeCalculation();
  void update(const SimpleEvent* const event);
  double measurementTime();
  TH1D* measurementTimeDistribution();
  bool isActive() const {return m_active;}
private:
  MeasurementTimeCalculation();
  bool m_active;
  double m_lastEventTime;
  QVector<double> m_positionInsideBin;
  QVector<int> m_positionInsideBinCounter;
  TH1D* m_timeDifference;
  TH1D* m_measurementTimeDistribution;
  bool m_deleteMeasurementTimeDistribution;
  Settings::Situation m_situation;
  void setFixTime(double);
};

#endif
