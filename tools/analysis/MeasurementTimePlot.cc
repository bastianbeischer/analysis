#include "MeasurementTimePlot.hh"
#include "SimpleEvent.hh"
#include "Settings.hh"
#include "SettingsManager.hh"

#include <TH1.h>
#include <TAxis.h>
#include <TPad.h>
#include <TLatex.h>

#include <cmath>

#include <QDebug>

MeasurementTimePlot::MeasurementTimePlot(const QDateTime& first, const QDateTime& last)
  : AnalysisPlot(AnalysisTopic::MomentumReconstruction)
  , H1DPlot()
  , m_calculation(first, last)
{
  setTitle("measurement time");
  TH1D* histogram = m_calculation.histogram();
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  histogram->SetMarkerSize(0.5);
  addHistogram(histogram, H1DPlot::P);

  addLatex(RootPlot::newLatex(.3, .85));
}

MeasurementTimePlot::~MeasurementTimePlot()
{
}

void MeasurementTimePlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  m_calculation.update(event);
}

void MeasurementTimePlot::update()
{
  double measurementTime = m_calculation.measurementTime();
  double timeError = m_calculation.measurementTimeError();
  latex()->SetTitle(qPrintable(QString("measurement time  = (%1 #pm %2) s").arg(measurementTime).arg(timeError)));
}

void MeasurementTimePlot::draw(TCanvas* canvas)
{
  if (m_drawn) {
    H1DPlot::draw(canvas);
  } else {
    H1DPlot::draw(canvas);
    //TODO check this, especially for older data !!!
    xAxis()->SetTimeOffset(3600, "gmt"); //dont understand this, but works at testbeam
    xAxis()->SetTimeDisplay(1);
    xAxis()->SetTimeFormat("%d-%H:%M");
    gPad->Modified();
    gPad->Update();
  }
}

