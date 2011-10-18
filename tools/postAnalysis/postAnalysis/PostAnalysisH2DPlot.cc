#include "PostAnalysisH2DPlot.hh"

#include <TAxis.h>
#include <TH2.h>

PostAnalysisH2DPlot::PostAnalysisH2DPlot(TH2D* histogram)
  : PostAnalysisPlot()
  , H2DPlot()
{
  init(QVector<TH2D*>() << histogram);
}

PostAnalysisH2DPlot::PostAnalysisH2DPlot(const QVector<TH2D*>& histograms)
  : PostAnalysisPlot()
  , H2DPlot()
{
  init(histograms);
}

PostAnalysisH2DPlot::~PostAnalysisH2DPlot()
{
}

void PostAnalysisH2DPlot::init(const QVector<TH2D*>& histograms)
{
  Q_ASSERT(histograms.count());
  setTitle(histograms[0]->GetTitle());
  setAxisTitle(histograms[0]->GetXaxis()->GetTitle(), histograms[0]->GetYaxis()->GetTitle(), histograms[0]->GetZaxis()->GetTitle());
  foreach(TH2D* histogram, histograms)
    addHistogram(histogram);
}
