#include "MeasurementTimePostAnalysisPlot.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLatex.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>
#include <QSettings>

MeasurementTimePostAnalysisPlot::MeasurementTimePostAnalysisPlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH1D* measurementTimeHistogram = new TH1D(*canvas->histograms1D().at(0));

  m_timeCalculation = new MeasurementTimeCalculation(measurementTimeHistogram);

  QString name = canvas->name();
  name.replace("canvas", "histogram");
  setTitle(name);
  addHistogram(m_timeCalculation->histogram());
  setAxisTitle(m_timeCalculation->histogram()->GetXaxis()->GetTitle(),m_timeCalculation->histogram()->GetYaxis()->GetTitle());
}

MeasurementTimePostAnalysisPlot::~MeasurementTimePostAnalysisPlot()
{
  if (m_timeCalculation) {
    delete m_timeCalculation;
    m_timeCalculation = 0;
  }
}

double MeasurementTimePostAnalysisPlot::measurementTime()
{
  return m_timeCalculation->measurementTime();
}
