#include "TrackFindingEfficiencyCorrection.hh"

#include "Corrections.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "Helpers.hh"

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

TrackFindingEfficiencyCorrection::TrackFindingEfficiencyCorrection(Type type, PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_type(type)
{
  m_typeNames.insert(Positive, "positive");
  m_typeNames.insert(Negative, "negative");
  
  TH1D* histogram = 0;
  if (m_type == Negative)
    histogram = Helpers::createMirroredHistogram(canvas->histograms1D().at(0));
  else
    histogram = new TH1D(*canvas->histograms1D().at(0));

  QString title = QString(canvas->name()).replace("canvas", "histogram");
  title.append(QString(" - ") + m_typeNames[m_type]);
  setTitle(title);

  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), "efficiency");

//  saveAsSetting();
}

TrackFindingEfficiencyCorrection::~TrackFindingEfficiencyCorrection()
{
}

void TrackFindingEfficiencyCorrection::saveAsSetting()
{
  if (m_type != Positive)
    return;
  EfficiencyCorrectionSettings::instance()->trackFindingEfficiency();
}
