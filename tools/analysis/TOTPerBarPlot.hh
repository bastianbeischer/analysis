#ifndef TOTPerBarPlot_hh
#define TOTPerBarPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>

class Hit;
class Track;
class SimpleEvent;

class TOTPerBarPlot : public AnalysisPlot, public H2DPlot {
public:
  TOTPerBarPlot();
  ~TOTPerBarPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  static const unsigned int nTofBars = 16;
};

#endif /* TOTPerBarPlot_hh */
