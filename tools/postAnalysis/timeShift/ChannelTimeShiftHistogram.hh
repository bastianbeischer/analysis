#ifndef ChannelTimeShiftHistogram_hh
#define ChannelTimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TF1;

class ChannelTimeShiftHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  ChannelTimeShiftHistogram(TCanvas*, int ch);
  virtual ~ChannelTimeShiftHistogram();
  virtual void draw(TCanvas*);
private:
  TF1* m_fitFunction;
};

#endif
