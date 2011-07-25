#ifndef H1DPostAnalysisPlot_hh
#define H1DPostAnalysisPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>

#include <TH1D.h>

class H1DPostAnalysisPlot : public PostAnalysisPlot, public H1DPlot {
public:
  H1DPostAnalysisPlot(TH1D* h1);
  H1DPostAnalysisPlot(TH1D* h1, TH1D* h2);
  H1DPostAnalysisPlot(TH1D* h1, TH1D* h2, TH1D* h3);
  virtual ~H1DPostAnalysisPlot();

private:

};

#endif
