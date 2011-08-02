#include "TrackFindingEfficiencyCorrection.hh"

#include "Corrections.hh"
#include "EfficiencyCorrectionSettings.hh"

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

TrackFindingEfficiencyCorrection::TrackFindingEfficiencyCorrection(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH1D* histogram = new TH1D(*canvas->histograms1D().at(0));

  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);

  addHistogram(histogram);
  setAxisTitle("abs(rigidity/GV)", "efficiency");
  m_name = QString(canvas->name()).remove("Track finding efficiency - ").remove(" canvas");

//  saveAsSetting();
}

TrackFindingEfficiencyCorrection::~TrackFindingEfficiencyCorrection()
{
}

void TrackFindingEfficiencyCorrection::saveAsSetting()
{
  EfficiencyCorrectionSettings effCorSet;
  effCorSet.save(EfficiencyCorrectionSettings::s_trackFindingEfficiencyPreKey, histogram());
}
