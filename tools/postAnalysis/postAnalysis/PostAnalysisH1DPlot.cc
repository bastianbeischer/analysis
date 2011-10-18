#include "PostAnalysisH1DPlot.hh"

#include <TAxis.h>

PostAnalysisH1DPlot::PostAnalysisH1DPlot(TH1D* histogram)
  : PostAnalysisPlot()
  , H1DPlot()
{
  init(QVector<TH1D*>() << histogram);
}

PostAnalysisH1DPlot::PostAnalysisH1DPlot(const QVector<TH1D*>& histograms)
  : PostAnalysisPlot()
  , H1DPlot()
{
  init(histograms);
}

PostAnalysisH1DPlot::~PostAnalysisH1DPlot()
{
}

void PostAnalysisH1DPlot::init(const QVector<TH1D*>& histograms)
{
  Q_ASSERT(histograms.count());
  setTitle(histograms[0]->GetTitle());
  setAxisTitle(histograms[0]->GetXaxis()->GetTitle(), histograms[0]->GetYaxis()->GetTitle());
  foreach (TH1D* histogram, histograms)
    addHistogram(new TH1D(*histogram));
}
