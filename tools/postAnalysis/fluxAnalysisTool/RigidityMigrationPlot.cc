#include "RigidityMigrationPlot.hh"

#include "PostAnalysisCanvas.hh"

#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>

RigidityMigrationPlot::RigidityMigrationPlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H2DPlot()
{
  TH2D* histogram = new TH2D(*canvas->histograms2D().at(0));
  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);
  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle(), histogram->GetZaxis()->GetTitle());
}

RigidityMigrationPlot::~RigidityMigrationPlot()
{
}
