#include "PMTCorrelationPlot.hh"

#include <TH2D.h>

#include "SimpleEvent.hh"
#include "SensorTypes.hh"

PMTCorrelationPlot::PMTCorrelationPlot() :
  AnalysisPlot(AnalysisPlot::Testbeam),
  H2DProjectionPlot()
{
  setTitle("cherenkov correlation");
  TH2D* histogram = new TH2D(qPrintable(title()), "", 150, -500, 3000, 150, -500, 3000);
  addHistogram(histogram);
}

PMTCorrelationPlot::~PMTCorrelationPlot()
{
}

void PMTCorrelationPlot::processEvent(const QVector<Hit*>&, Particle*, SimpleEvent* event)
{
  histogram()->Fill(event->sensorData(SensorTypes::BEAM_CHERENKOV1), event->sensorData(SensorTypes::BEAM_CHERENKOV2));
}
