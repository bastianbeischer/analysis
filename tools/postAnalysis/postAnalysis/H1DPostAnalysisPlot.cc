#include "H1DPostAnalysisPlot.hh"

#include <TAxis.h>

H1DPostAnalysisPlot::H1DPostAnalysisPlot(TH1D* histogram)
: PostAnalysisPlot()
, H1DPlot()
{
  setTitle(histogram->GetTitle());
  addHistogram(new TH1D(*histogram));
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
}

H1DPostAnalysisPlot::H1DPostAnalysisPlot(TH1D* h1, TH1D* h2)
: PostAnalysisPlot()
, H1DPlot()
{
  setTitle(h1->GetTitle());
  addHistogram(new TH1D(*h1));
  setAxisTitle(h1->GetXaxis()->GetTitle(), h1->GetYaxis()->GetTitle());
  addHistogram(new TH1D(*h2));
}

H1DPostAnalysisPlot::H1DPostAnalysisPlot(TH1D* h1, TH1D* h2, TH1D* h3)
: PostAnalysisPlot()
, H1DPlot()
{
  setTitle(h1->GetTitle());
  addHistogram(new TH1D(*h1));
  setAxisTitle(h1->GetXaxis()->GetTitle(), h1->GetYaxis()->GetTitle());
  addHistogram(new TH1D(*h2));
  addHistogram(new TH1D(*h3));
}

H1DPostAnalysisPlot::~H1DPostAnalysisPlot()
{

}
