#ifndef TOTIonizationCorrelation_hh
#define TOTIonizationCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "Hit.hh"
#include "TOTLayer.hh"

#include <QVector>
#include <QString>

class Particle;
class Track;
class SimpleEvent;

class TOTIonizationCorrelation : public H2DPlot, public TOTLayer {
public:
  TOTIonizationCorrelation(Hit::ModuleType type);
  TOTIonizationCorrelation(TOTLayer::Layer layer, Hit::ModuleType type);
  ~TOTIonizationCorrelation();
  TOTIonizationCorrelation* create(TOTLayer::Layer layer) const;
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
private:
  double sumOfNonTOFSignalHeights(const Track*, const QVector<Hit*>&);
  Hit::ModuleType m_type;
};

#endif /* TOTIonizationCorrelation_hh */
