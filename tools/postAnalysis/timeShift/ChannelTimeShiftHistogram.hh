#ifndef ChannelTimeShiftHistogram_hh
#define ChannelTimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;

class ChannelTimeShiftHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  ChannelTimeShiftHistogram(PostAnalysisCanvas*, int ch);
  virtual ~ChannelTimeShiftHistogram();
};

#endif
