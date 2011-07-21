#ifndef H1Plot_hh
#define H1Plot_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>

#include <TH1D.h>

class H1Plot : public PostAnalysisPlot, public H1DPlot {
public:
  H1Plot(TH1D* histogram);
  H1Plot(TH1D* histogram1, TH1D* histogram2);
  virtual ~H1Plot();

private:

};

#endif
