#include "H2DPostAnalysisPlot.hh"

#include <TAxis.h>

H2DPostAnalysisPlot::H2DPostAnalysisPlot(TH2D* histogram)
  : PostAnalysisPlot()
  , H2DPlot()
{
  setTitle(histogram->GetTitle());
  addHistogram(new TH2D(*histogram));
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle(), histogram->GetZaxis()->GetTitle());
}

H2DPostAnalysisPlot::~H2DPostAnalysisPlot()
{

}
