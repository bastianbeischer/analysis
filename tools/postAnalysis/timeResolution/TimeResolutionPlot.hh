#ifndef TimeResolutionPlot_hh
#define TimeResolutionPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeResolutionAnalysis;
class PostAnalysisCanvas;

class TimeResolutionPlot : public PostAnalysisPlot, public H1DPlot {
public:
  enum Type {Variance, StandardDeviation};
  TimeResolutionPlot(const TimeResolutionAnalysis* const, Type, PostAnalysisCanvas* = 0);
  virtual ~TimeResolutionPlot();
};

#endif
