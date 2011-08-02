#include "AllTrackerLayersFlagEfficiencyCorrection.hh"

#include "Corrections.hh"
#include "EfficiencyCorrectionSettings.hh"

#include <TH1.h>
#include <TH2.h>
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

AllTrackerLayersFlagEfficiencyCorrection::AllTrackerLayersFlagEfficiencyCorrection(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH1D* histogram = new TH1D(*canvas->histograms1D().at(0));

  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);

  addHistogram(histogram);
  setAxisTitle("abs(rigidity/GV)", "efficiency");
  m_name = QString(canvas->name()).remove("One hit in all layers ").remove(" canvas");

//  saveAsSetting();
}

AllTrackerLayersFlagEfficiencyCorrection::~AllTrackerLayersFlagEfficiencyCorrection()
{
}

void AllTrackerLayersFlagEfficiencyCorrection::saveAsSetting()
{
  EfficiencyCorrectionSettings effCorSet;
  effCorSet.save(EfficiencyCorrectionSettings::s_allTrackerLayerCutEfficiencyPreKey, histogram());
}
