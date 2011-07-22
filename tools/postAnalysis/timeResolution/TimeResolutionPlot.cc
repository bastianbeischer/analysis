#include "TimeResolutionPlot.hh"
#include "TimeOfFlightHistogram.hh"

#include <TGraph.h>

TimeResolutionPlot::TimeResolutionPlot(const QVector<TimeOfFlightHistogram*>& plots)
  : PostAnalysisPlot()
  , GraphPlot()
{
  setTitle("time resolution plot " + plots[0]->title().mid(16, 27));
  QVector<double> y;
  QVector<double> sigma;
  TGraph* graph = new TGraph;
  foreach (TimeOfFlightHistogram* plot, plots)
    graph->SetPoint(graph->GetN(), plot->upperY(), plot->sigma());
  setDrawOption(ALP);
  addGraph(graph);
}

TimeResolutionPlot::~TimeResolutionPlot()
{}
