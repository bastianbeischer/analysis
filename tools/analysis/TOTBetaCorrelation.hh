#ifndef TOTBetaCorrelation_hh
#define TOTBetaCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "TOTLayerPlot.hh"

#include <QString>

class TOTBetaCorrelation : public H2DPlot, public TOTLayerPlot {
public:
  TOTBetaCorrelation(const QString& title, TOTLayerPlot::Layer);
  ~TOTBetaCorrelation();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
};

#endif
