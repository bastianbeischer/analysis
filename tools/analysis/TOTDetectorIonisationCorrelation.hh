#ifndef TOTDetectorIonisationCorrelation_hh
#define TOTDetectorIonisationCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "Hit.hh"

#include <QVector>
#include <QString>

class Track;
class SimpleEvent;

class TOTDetectorIonisationCorrelation : public AnalysisPlot, public H2DPlot {
public:
  enum TofLayer {
    LOWER, UPPER, TOTAL
  };
  TOTDetectorIonisationCorrelation(TofLayer layer, Hit::ModuleType type);
  ~TOTDetectorIonisationCorrelation();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  void draw(TCanvas* canvas);
private:
  TofLayer m_layer;
  QString layerName(TofLayer layer);
  bool checkLayer(double z);
  double sumOfNonTOFSignalHeights(Track* track, QVector<Hit*> clusters);
  Hit::ModuleType m_type;
};

#endif /* TOTDetectorIonisationCorrelation_hh */
