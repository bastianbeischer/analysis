#ifndef TimeShiftHistogram_hh
#define TimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeShiftHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  TimeShiftHistogram(TCanvas*, int ch);
  virtual ~TimeShiftHistogram();
};

#endif
