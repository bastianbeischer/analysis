#ifndef TOTTimeCorrelationPlotCollection_hh
#define TOTTimeCorrelationPlotCollection_hh

#include "PlotCollection.hh"

#include <QDateTime>

class TOTTimeCorrelationPlotCollection : public PlotCollection
{
public:
  TOTTimeCorrelationPlotCollection(const QDateTime&, const QDateTime&);
  ~TOTTimeCorrelationPlotCollection();
};

#endif /* TOTTimeCorrelationPlotCollection_hh */
