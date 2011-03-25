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
  TOTBetaCorrelation(QString layer);
  ~TOTBetaCorrelation();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void update();
  void draw(TCanvas* canvas);
private:
  QString m_layer;
  bool checkLayer(double z);
};

#endif
