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
  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  int nBins = qMin((t2 - t1) / 60, 200);
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBins, t1, t2, 396, -99, 99);
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
