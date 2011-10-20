#ifndef TOTProjectionPlot_hh
#define TOTProjectionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "TOTLayerPlot.hh"

#include <QVector>

class TOTProjectionPlot : public H1DPlot, public TOTLayerPlot {
public:
  TOTProjectionPlot();
  TOTProjectionPlot(const QString& title, TOTLayerPlot::Layer);
  ~TOTProjectionPlot();
  virtual void processEvent(const AnalyzedEvent*);
};

#endif /* TOTProjectionPlot_hh */
