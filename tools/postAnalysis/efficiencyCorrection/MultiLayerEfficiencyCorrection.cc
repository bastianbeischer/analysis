#include "MultiLayerEfficiencyCorrection.hh"

#include "Corrections.hh"

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

MultiLayerEfficiencyCorrection::MultiLayerEfficiencyCorrection(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH2D* h2 = new TH2D(*canvas->histograms2D().at(0));

  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);

  TH1D* histogram = h2->ProjectionX(qPrintable(title+"projection"), 8, 8);

  addHistogram(histogram, H1DPlot::HIST);
  setAxisTitle("abs(rigidity/GV)", "efficiency");
  m_name = QString(canvas->name()).remove("Multi Layer Efficiency ").remove(" canvas");
  
  saveAsSetting();
}

MultiLayerEfficiencyCorrection::~MultiLayerEfficiencyCorrection()
{
}

void MultiLayerEfficiencyCorrection::saveAsSetting()
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
  
  //for now only valid for flight configuration
  path += "kiruna/";
  
  QSettings* settings = new QSettings(path + "EfficiencyCorrections.conf", QSettings::IniFormat);
  
  QString prefix = "multiLayerEfficiency_"+m_name;
  settings->setValue(prefix+"/axis", axis);
  settings->setValue(prefix+"/values", values);
  settings->sync();
  
  delete settings;
  settings = 0;
}
