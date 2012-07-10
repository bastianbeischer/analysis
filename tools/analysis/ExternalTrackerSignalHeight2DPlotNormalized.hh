#ifndef ExternalTrackerSignalHeight2DPlotNormalized_hh
#define ExternalTrackerSignalHeight2DPlotNormalized_hh

#include "ExternalTrackerSignalHeight2DPlot.hh"

class ExternalTrackerSignalHeight2DPlotNormalized : public ExternalTrackerSignalHeight2DPlot {
public:
  ExternalTrackerSignalHeight2DPlotNormalized(unsigned short moduleId);
  ~ExternalTrackerSignalHeight2DPlotNormalized();
  void update();

};

#endif /* ExternalTrackerSignalHeight2DPlotNormalized_hh */
