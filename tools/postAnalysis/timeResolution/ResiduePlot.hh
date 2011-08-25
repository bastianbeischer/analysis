#ifndef ResiduePlot_hh
#define ResiduePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeResolutionAnalysis;
class TimeOfFlightHistogram;

class ResiduePlot : public PostAnalysisPlot, public H1DPlot {
public:
  ResiduePlot(const QVector<TimeOfFlightHistogram*>&, const TimeResolutionAnalysis* const);
  virtual ~ResiduePlot();
};

#endif
