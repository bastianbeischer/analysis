#ifndef H2DPostAnalysisPlot_hh
#define H2DPostAnalysisPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QString>

#include <TH2D.h>

class H2DPostAnalysisPlot : public PostAnalysisPlot, public H2DPlot {
public:
  H2DPostAnalysisPlot(TH2D* histogram);
  virtual ~H2DPostAnalysisPlot();

private:

};

#endif
