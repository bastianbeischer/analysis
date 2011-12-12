#ifndef MeasurementTimePlot_hh
#define MeasurementTimePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QDateTime>

class MeasurementTimePlot : public AnalysisPlot, public H1DPlot {
public:
  MeasurementTimePlot(const QDateTime& first, const QDateTime& last);
  virtual ~MeasurementTimePlot();
  void processEvent(const AnalyzedEvent*);
  void update();
  void draw(TCanvas* canvas);
};

#endif
