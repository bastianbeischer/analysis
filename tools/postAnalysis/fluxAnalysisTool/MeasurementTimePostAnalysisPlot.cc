#include "MeasurementTimePostAnalysisPlot.hh"

#include "PostAnalysisCanvas.hh"

#include <TH1D.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TAxis.h>

#include <QDebug>

MeasurementTimePostAnalysisPlot::MeasurementTimePostAnalysisPlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH1D* measurementTimeHistogram = new TH1D(*canvas->histograms1D().at(0));

  m_timeCalculation = new MeasurementTimeCalculation(measurementTimeHistogram);

  QString name = canvas->name();
  name.replace("canvas", "histogram");
  setTitle(name);
  addHistogram(m_timeCalculation->measurementTimeDistribution());
  setAxisTitle(m_timeCalculation->measurementTimeDistribution()->GetXaxis()->GetTitle(), m_timeCalculation->measurementTimeDistribution()->GetYaxis()->GetTitle());
}

MeasurementTimePostAnalysisPlot::~MeasurementTimePostAnalysisPlot()
{
  if (m_timeCalculation)
    delete m_timeCalculation;
}

double MeasurementTimePostAnalysisPlot::measurementTime()
{
  return m_timeCalculation->measurementTime();
}
