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
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
  virtual void draw(TCanvas* canvas);
protected:
  SensorTypes::Type m_type;
  TH1D* m_normalizationHistogram;
};

#endif
