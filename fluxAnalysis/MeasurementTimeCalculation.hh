#ifndef MeasurementTimeCalculation_hh
#define MeasurementTimeCalculation_hh

#include "SimpleEvent.hh"

#include <TH1D.h>

#include <QDateTime>
#include <QMutex>

class MeasurementTimeCalculation {
	
public:
  MeasurementTimeCalculation();
	MeasurementTimeCalculation(QDateTime first, QDateTime last);
  MeasurementTimeCalculation(TH1D* histogram);
	~MeasurementTimeCalculation();

  void update(const SimpleEvent* const event);
  TH1D* histogram();
  double measurementTime();
  double measurementTimeError();

private:
  TH1D* m_histogram;
  double m_measurementTime;
  double m_measurementTimeError;
  QMutex m_mutex;
};


#endif
