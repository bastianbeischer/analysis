#include "TrackingEfficiencyCorrection.hh"

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

TrackingEfficiencyCorrection::TrackingEfficiencyCorrection(PostAnalysisCanvas* canvas)
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

  fit();
  saveAsSetting();
}

TrackingEfficiencyCorrection::~TrackingEfficiencyCorrection()
{
  if (m_efficiencyFit) {
    delete m_efficiencyFit;
    m_efficiencyFit = 0;
  }
}

Double_t myfunction(Double_t *x, Double_t *par)
{
  Double_t xx = x[0];
  Double_t f = par[0] + par[1] * exp(-1 * (par[6] * xx - par[2])) - par[3] / (par[4] * xx - par[5]);
  return f;
}

void TrackingEfficiencyCorrection::fit()
{
  m_efficiencyFit = new TF1("myfunc",myfunction,0.001,20,7);
  m_efficiencyFit->SetLineColor(kRed);

  m_efficiencyFit->SetParameter(4, 2.1);
  m_efficiencyFit->SetParLimits(4, 2, 2.4);

  histogram(0)->Fit(m_efficiencyFit,"EQN0");
  
  addFunction(m_efficiencyFit);
}

void TrackingEfficiencyCorrection::saveAsSetting()
{
  QList<QVariant> params;
  for (int i = 0; i <  m_efficiencyFit->GetNumberFreeParameters(); ++i) {
    params.push_back( m_efficiencyFit->GetParameter(i) );
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
  
  QString prefix = "trackingEfficiencyParam/";
  settings->setValue(prefix+m_name, params);
  settings->sync();
  
  delete settings;
  settings = 0;
}
