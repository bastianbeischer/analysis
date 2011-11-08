#ifndef SignalHeightTimeCorrelationPlotCollection_hh
#define SignalHeightTimeCorrelationPlotCollection_hh

#include "PlotCollection.hh"

#include <QDateTime>

class SignalHeightTimeCorrelationPlotCollection : public PlotCollection {
public:
  SignalHeightTimeCorrelationPlotCollection(const QDateTime&, const QDateTime&);
  ~SignalHeightTimeCorrelationPlotCollection();
};

#endif /* SignalHeightTimeCorrelationPlotCollection_hh */
