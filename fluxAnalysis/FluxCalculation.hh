#ifndef FluxCalculation_hh
#define FluxCalculation_hh

#include "SimpleEvent.hh"

#include <TH1D.h>
#include <TLatex.h>

#include <QVector>
#include <QString>
#include <QMutex>

class FluxCalculation {
public:
  FluxCalculation(TH1D* particleHistogram, double measurementTime);
	~FluxCalculation();
  void setMeasurementTime(double measurementTime);
  void update();
  static void efficiencyCorrection(TH1D* histogramToCorrect, double efficiency);
  static void efficiencyCorrection(TH1D* histogramToCorrect, TH1D* efficiencyHistogram);
  void efficiencyCorrection(TH1D* efficiencyHistogram);
  void efficiencyCorrection(double efficiency);
  TH1D* fluxHistogram() const {return m_fluxHistogram;}
  TLatex binTitle(int bin);
private:
  FluxCalculation();
  TH1D* m_particleHistogram;
  double m_measurementTime;
  TH1D* m_fluxHistogram;
  QMutex m_mutexUpdate;
  QMutex m_mutexBinTitle;
};


#endif
