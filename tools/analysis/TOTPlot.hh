#ifndef TOTPlot_hh
#define TOTPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>

class Hit;
class Particle;
class SimpleEvent;

class TOTPlot : public AnalysisPlot, public H2DPlot {
public:
  TOTPlot();
  ~TOTPlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
};

#endif /* TOTPlot_hh */
