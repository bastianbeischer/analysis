#ifndef MeasurementTimeDistributionPlot_hh
#define MeasurementTimeDistributionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class MeasurementTimeDistributionPlot : public AnalysisPlot, public H1DPlot {
public:
  MeasurementTimeDistributionPlot(int numberOfThreads);
  virtual ~MeasurementTimeDistributionPlot();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
  void finalize();
private:
  double m_lastEventTime;
  bool m_active;
};

#endif
