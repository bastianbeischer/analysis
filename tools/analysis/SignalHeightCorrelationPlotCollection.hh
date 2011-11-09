#ifndef SignalHeightCorrelationPlotCollection_hh
#define SignalHeightCorrelationPlotCollection_hh

#include "PlotCollection.hh"
#include "SignalHeightCorrelationPlot.hh"

#include <QDateTime>

class SignalHeightCorrelationPlotCollection : public PlotCollection {
public:
  SignalHeightCorrelationPlotCollection(SignalHeightCorrelationPlot::SensorType, const QDateTime& = QDateTime(), const QDateTime& = QDateTime());
  ~SignalHeightCorrelationPlotCollection();
};

#endif /* SignalHeightCorrelationPlotCollection_hh */
