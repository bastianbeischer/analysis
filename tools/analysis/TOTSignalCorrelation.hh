#ifndef TOTSignalCorrelation_hh
#define TOTSignalCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "Hit.hh"
#include "TOTLayerPlot.hh"

#include <QVector>
#include <QString>

class Particle;
class Track;
class SimpleEvent;

class TOTSignalCorrelation : public H2DPlot, public TOTLayerPlot {
public:
  TOTSignalCorrelation(const QString& title, Hit::ModuleType, TOTLayerPlot::Layer);
  ~TOTSignalCorrelation();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
private:
  double sumOfNonTOFSignalHeights(const QVector<Hit*>&);
  Hit::ModuleType m_type;
};

#endif /* TOTSignalCorrelation_hh */
