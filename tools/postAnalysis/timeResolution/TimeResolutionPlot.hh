#ifndef TimeResolutionPlot_hh
#define TimeResolutionPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

class TimeResolutionAnalysis;

class TimeResolutionPlot : public PostAnalysisPlot, public GraphPlot {
public:
  enum Type {Variance, StandardDeviation};
  TimeResolutionPlot(const TimeResolutionAnalysis* const, Type);
  virtual ~TimeResolutionPlot();
};

#endif
