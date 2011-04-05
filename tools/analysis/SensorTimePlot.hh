#ifndef SensorTimePlot_hh
#define SensorTimePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "SensorTypes.hh"

#include <QDateTime>

class SimpleEvent;
class TH1D;

class SensorTimePlot : public AnalysisPlot, public H1DPlot {
public:
  SensorTimePlot(SensorTypes::Type, QDateTime first, QDateTime last);
  virtual ~SensorTimePlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
protected:
  SensorTypes::Type m_type;
  TH1D* m_normalizationHistogram;
};

#endif
