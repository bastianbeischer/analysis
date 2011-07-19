#include "TrackFindingEfficiencyCorrection.hh"

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

TrackFindingEfficiencyCorrection::TrackFindingEfficiencyCorrection(PostAnalysisCanvas* canvas, QString config)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH1D* histogram = new TH1D(*canvas->histograms1D().at(0));

  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);

  addHistogram(histogram);
  setAxisTitle("abs(rigidity/GV)", "efficiency");
  m_name = QString(canvas->name()).remove("Track finding efficiency - ").remove(" canvas");

//  saveAsSetting(config);
}

TrackFindingEfficiencyCorrection::~TrackFindingEfficiencyCorrection()
{
}

void TrackFindingEfficiencyCorrection::saveAsSetting(QString config)
{
  QList<QVariant> axis;
  for (int i = 0; i <=  histogram()->GetNbinsX(); ++i) {
    axis.push_back( histogram()->GetBinLowEdge(i+1) );
  }

  QList<QVariant> values;
  for (int i = 0; i <  histogram()->GetNbinsX(); ++i) {
    double value = histogram()->GetBinContent(i+1);
    if (value == 0)
      value = 1;
    values.push_back( value );
  }

  const char* env = getenv("PERDAIXANA_PATH");
  if (env == 0) {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  QString path(env);
  path += "/conf/";

  path += config+"/";

  QSettings* settings = new QSettings(path + "EfficiencyCorrections.conf", QSettings::IniFormat);

  QString prefix = "trackFindingEfficiency_"+m_name;
  settings->setValue(prefix+"/axis", axis);
  settings->setValue(prefix+"/values", values);
  settings->sync();

  delete settings;
  settings = 0;
}
