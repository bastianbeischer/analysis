#include "AzimuthMigration.hh"

#include "Corrections.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>
#include <TMultiGraph.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>
#include <QSettings>
#include <QVector>

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
