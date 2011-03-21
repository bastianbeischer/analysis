#ifndef TimeShiftHistogram_hh
#define TimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TF1;

class TimeShiftHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  TimeShiftHistogram(TCanvas*, int ch);
  virtual ~TimeShiftHistogram();
  virtual void draw(TCanvas*);
private:
  TF1* m_fitFunction;
};

#endif
