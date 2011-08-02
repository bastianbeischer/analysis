#ifndef MeasurementTimePlot_hh
#define MeasurementTimePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "MeasurementTimeCalculation.hh"

#include <QDateTime>

class MeasurementTimePlot : public AnalysisPlot, public H1DPlot {
public:
  MeasurementTimePlot(QDateTime first, QDateTime last);
  virtual ~MeasurementTimePlot();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
  void finalize();
  void draw(TCanvas* canvas);

private:
  MeasurementTimeCalculation m_calculation;
};

#endif
