#ifndef TOTBetaCorrelation_hh
#define TOTBetaCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QString>

class Hit;
class Track;
class SimpleEvent;

class TOTBetaCorrelation : public AnalysisPlot, public H2DPlot {
public:
  enum TofLayer {Lower, Upper, All};
  TOTBetaCorrelation(TofLayer layer);
  ~TOTBetaCorrelation();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
private:
  TofLayer m_layer;
  QString layerName(TofLayer layer);
  bool checkLayer(double z);
};

#endif
