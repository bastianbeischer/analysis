#include "TemperatureTimePlot.hh"
#include "Hit.hh"
#include "SensorTypes.hh"
#include "SimpleEvent.hh"

#include <TH2.h>
#include <TAxis.h>

#include <QDebug>

TemperatureTimePlot::TemperatureTimePlot(SensorTypes::Type type, QDateTime first, QDateTime last)
  : AnalysisPlot(AnalysisPlot::SlowControl)
  , H2DPlot()
  , m_type(type)
{
  setTitle(QString::fromStdString(SensorTypes::convertToString(m_type)));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 100, first.toTime_t(), last.toTime_t(), 396, -99, 99);
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
  histogram->GetXaxis()->SetTitle("time");
  histogram->GetYaxis()->SetTitle("#vartheta / #circC");
  setHistogram(histogram);
}

TemperatureTimePlot::~TemperatureTimePlot()
{}

void TemperatureTimePlot::processEvent(const QVector<Hit*>&, Track*, SimpleEvent* event)
{
  histogram()->Fill(event->time(), event->sensorData(m_type));
}
