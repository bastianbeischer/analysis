#include "TrackerTemperatureTimePlot.hh"
#include "Hit.hh"
#include "SensorTypes.hh"
#include "SimpleEvent.hh"

#include <TH2.h>
#include <TAxis.h>

#include <QDebug>

TrackerTemperatureTimePlot::TrackerTemperatureTimePlot(QDateTime first, QDateTime last)
  : AnalysisPlot(AnalysisPlot::SlowControl)
  , H2DPlot()
  , m_first(first)
  , m_last(last)
{
  setTitle(QString("tracker temperature"));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 100, first.toTime_t(), last.toTime_t(), 100, -10, 40);
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d %H:%M");
  histogram->GetXaxis()->LabelsOption("v");
  histogram->GetXaxis()->SetTitle("time");
  histogram->GetYaxis()->SetTitle("#vartheta / #circC");
  setHistogram(histogram);
}

TrackerTemperatureTimePlot::~TrackerTemperatureTimePlot()
{}

void TrackerTemperatureTimePlot::processEvent(const QVector<Hit*>&, Track*, SimpleEvent* event)
{
  histogram()->Fill(event->time(), event->sensorData(SensorTypes::HPE_0x3000_TEMP));
}
