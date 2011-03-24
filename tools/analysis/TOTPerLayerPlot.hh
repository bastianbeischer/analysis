#ifndef TOTPerLayerPlot_hh
#define TOTPerLayerPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QVector>
#include <QString>

class Hit;
class Track;
class SimpleEvent;

class TOTPerLayerPlot : public AnalysisPlot, public H1DPlot {
public:
  enum TofLayer {
    LOWER, UPPER, TOTAL
  };
  TOTPerLayerPlot(TofLayer layer);
  ~TOTPerLayerPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
private:
  TofLayer m_layer;
  QString layerName(TofLayer layer);
  bool checkLayer(double z);
};

#endif /* TOTPerLayerPlot_hh */
