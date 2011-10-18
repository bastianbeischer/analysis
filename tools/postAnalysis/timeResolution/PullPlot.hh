#ifndef PullPlot_hh
#define PullPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeResolutionAnalysis;
class TimeOfFlightHistogram;

class PullPlot : public PostAnalysisPlot, public H1DPlot {
public:
  PullPlot(const QVector<TimeOfFlightHistogram*>&, const TimeResolutionAnalysis* const);
  virtual ~PullPlot();
};

#endif
