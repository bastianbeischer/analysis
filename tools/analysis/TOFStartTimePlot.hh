#ifndef TOFStartTimePlot_hh
#define TOFStartTimePlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QVector>

class AnalyzedEvent;

class TOFStartTimePlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  TOFStartTimePlot();
  ~TOFStartTimePlot();
  virtual void processEvent(const AnalyzedEvent*);
};

#endif /* TOFStartTimePlot_hh */
