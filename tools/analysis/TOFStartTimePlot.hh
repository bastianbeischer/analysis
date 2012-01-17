#ifndef TOFStartTimePlot_hh
#define TOFStartTimePlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QVector>

class Hit;
class Particle;
class SimpleEvent;

class TOFStartTimePlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  TOFStartTimePlot();
  ~TOFStartTimePlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
};

#endif /* TOFStartTimePlot_hh */
