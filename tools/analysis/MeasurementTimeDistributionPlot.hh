#ifndef MeasurementTimeDistributionPlot_hh
#define MeasurementTimeDistributionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;

class MeasurementTimeDistributionPlot : public AnalysisPlot, public H1DPlot {
public:
  MeasurementTimeDistributionPlot(int numberOfThreads);
  virtual ~MeasurementTimeDistributionPlot();
  void processEvent(const AnalyzedEvent*);
  virtual void update();
private:
  bool m_active;
  double m_lastEventTime;
  QVector<double> m_positionInsideBin;
  QVector<int> m_positionInsideBinCounter;
  TH1D* m_histogram;
};

#endif
