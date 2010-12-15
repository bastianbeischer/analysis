#ifndef TimeDifferenceHistogram_hh
#define TimeDifferenceHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TF1;

class TimeDifferenceHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  TimeDifferenceHistogram(TCanvas*, int ch);
  virtual ~TimeDifferenceHistogram();
  virtual void draw(TCanvas*);
private:
  TF1* m_fitFunction;
};

#endif
