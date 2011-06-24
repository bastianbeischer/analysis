#ifndef TimeResolutionPlot_hh
#define TimeResolutionPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

class TimeOfFlightHistogram;

class TimeResolutionPlot : public PostAnalysisPlot, public GraphPlot {
public:
  TimeResolutionPlot(const QVector<TimeOfFlightHistogram*>&);
  virtual ~TimeResolutionPlot();
};

#endif
