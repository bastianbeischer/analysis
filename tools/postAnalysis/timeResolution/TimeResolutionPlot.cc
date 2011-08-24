#include "TimeResolutionPlot.hh"
#include "TimeResolutionAnalysis.hh"

#include <TGraph.h>
#include <TAxis.h>

#include <QDebug>

TimeResolutionPlot::TimeResolutionPlot(const TimeResolutionAnalysis* const analysis, Type type)
  : PostAnalysisPlot()
  , GraphPlot()
{
  QString title = "time resolution plot";
  title.append((type == Variance) ? QString(" variance") : QString(" standard deviation"));
  setTitle(title);
  
  int nBins = analysis->nBins();

  for (int i = 0; i < 4; ++i) {
    TGraph* graph = new TGraph(nBins);
    graph->SetLineStyle(1);
    graph->SetLineColor(1+i);
    graph->SetName(qPrintable(QString("i = %1").arg(i)));
    for (int k = 0; k < nBins; ++k)
      graph->SetPoint(k, analysis->y(k), (type == Variance) ? analysis->vIK(i, k) : analysis->sigmaIK(i, k));
    setDrawOption(ALP);
    addGraph(graph, LP);
  }

  for (int j = 0; j < 4; ++j) {
    TGraph* graph = new TGraph(nBins);
    graph->SetLineStyle(2);
    graph->SetLineColor(1+j);
    graph->SetName(qPrintable(QString("j = %1").arg(j)));
    for (int l = 0; l < nBins; ++l)
      graph->SetPoint(l, analysis->y(l), (type == Variance) ? analysis->vJL(j, l) : analysis->sigmaJL(j, l));
    setDrawOption(ALP);
    addGraph(graph, LP);
  }

  yAxis()->SetRangeUser((type == Variance) ? -1. : 0., 1.);
  setAxisTitle("y / mm", (type == Variance) ? QString("variance / ns^{2}") : QString("#sigma / ns"));
}

TimeResolutionPlot::~TimeResolutionPlot()
{}
