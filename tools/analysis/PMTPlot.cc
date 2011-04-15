#include "PMTPlot.hh"

#include <TH1D.h>

#include "SimpleEvent.hh"
#include "SensorTypes.hh"

PMTPlot::PMTPlot(SensorTypes::Type type) :
  AnalysisPlot(AnalysisPlot::Testbeam),
  H1DPlot(),
  m_type(type)
{
  setTitle(SensorTypes::convertToString(m_type));
  TH1D* histogram = new TH1D(qPrintable(title()), "", 500, -100, 2000);
  addHistogram(histogram);
}

PMTPlot::~PMTPlot()
{
}

void PMTPlot::processEvent(const QVector<Hit*>&, Particle*, SimpleEvent* event)
{
  histogram()->Fill(event->sensorData(m_type));
}
