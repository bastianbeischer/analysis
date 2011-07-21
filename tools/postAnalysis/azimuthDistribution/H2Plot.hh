#ifndef H2Plot_hh
#define H2Plot_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QString>

#include <TH2D.h>
#include <TH1D.h>

class H2Plot : public PostAnalysisPlot, public H2DPlot {
public:
  H2Plot(TH2D* histogram);
  virtual ~H2Plot();

private:

};

#endif
