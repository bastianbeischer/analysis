#include "AzimuthMigration.hh"
#include "PostAnalysisCanvas.hh"

#include <TH2.h>
#include <TAxis.h>

#include <cmath>

AzimuthMigration::AzimuthMigration(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H2DPlot()
{
  TH2D* histogram = new TH2D(*canvas->histograms2D().at(0));
  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);
  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle(), histogram->GetZaxis()->GetTitle());
}

AzimuthMigration::~AzimuthMigration()
{
}
