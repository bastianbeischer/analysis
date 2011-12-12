#ifndef SignalHeightCorrelationPlotCollection_hh
#define SignalHeightCorrelationPlotCollection_hh

#include "PlotCollection.hh"
#include "SignalHeightCorrelationPlot.hh"

#include <QDateTime>

class SignalHeightCorrelationPlotCollection : public PlotCollection {
public:
  SignalHeightCorrelationPlotCollection(SignalHeightCorrelationPlot::CorrelationType);
  SignalHeightCorrelationPlotCollection(SignalHeightCorrelationPlot::CorrelationType, const QDateTime& first, const QDateTime& last);
  ~SignalHeightCorrelationPlotCollection();
};

#endif /* SignalHeightCorrelationPlotCollection_hh */
