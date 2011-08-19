#ifndef MeasurementTimeCalculation_hh
#define MeasurementTimeCalculation_hh

#include <QDateTime>

class TH1D;
class SimpleEvent;

// Simple approach to determine dead times.
// A more suffisticated approach has yet to
// be developed.
// measurementTimeError is certainly not
// correct!
class MeasurementTimeCalculation {
public:
	MeasurementTimeCalculation(const QDateTime& first, const QDateTime& last);
  MeasurementTimeCalculation(TH1D* histogram);
	~MeasurementTimeCalculation();
  void update(const SimpleEvent* const event);
  TH1D* histogram() const;
  double measurementTime() const;
  double measurementTimeError() const;
private:
  MeasurementTimeCalculation();
  TH1D* m_histogram;
  double m_measurementTimeError;
};

#endif
