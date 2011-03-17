#ifndef TrackerTemperatureTimePlot_hh
#define TrackerTemperatureTimePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QDateTime>

class TrackerTemperatureTimePlot : public AnalysisPlot, public H2DPlot {
public:
  TrackerTemperatureTimePlot(QDateTime first, QDateTime last);
  virtual ~TrackerTemperatureTimePlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
private:
  QDateTime m_first;
  QDateTime m_last;
};

#endif
