#ifndef TOTPlot_hh
#define TOTPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QVector>

class Hit;
class Particle;
class SimpleEvent;

class TOTPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  TOTPlot();
  ~TOTPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
};

#endif /* TOTPlot_hh */
