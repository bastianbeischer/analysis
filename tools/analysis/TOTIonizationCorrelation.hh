#ifndef TOTIonizationCorrelation_hh
#define TOTIonizationCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "Hit.hh"

#include <QVector>
#include <QString>

class Track;
class SimpleEvent;

class TOTIonizationCorrelation : public AnalysisPlot, public H2DPlot {
public:
  enum TofLayer {Lower, Upper, All};
  TOTIonizationCorrelation(TofLayer layer, Hit::ModuleType type);
  ~TOTIonizationCorrelation();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
private:
  TofLayer m_layer;
  QString layerName(TofLayer layer);
  bool checkLayer(double z);
  double sumOfNonTOFSignalHeights(Track*, const QVector<Hit*>&);
  Hit::ModuleType m_type;
};

#endif /* TOTIonizationCorrelation_hh */
