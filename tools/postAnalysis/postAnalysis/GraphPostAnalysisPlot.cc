#include "GraphPostAnalysisPlot.hh"

#include <TAxis.h>

GraphPostAnalysisPlot::GraphPostAnalysisPlot(TGraph* graph)
: PostAnalysisPlot()
, GraphPlot()
{
  setTitle(graph->GetTitle());
  addGraph(new TGraph(*graph));
  setAxisTitle(graph->GetXaxis()->GetTitle(), graph->GetYaxis()->GetTitle());
}

GraphPostAnalysisPlot::GraphPostAnalysisPlot(TGraph* graph1, TGraph* graph2)
: PostAnalysisPlot()
, GraphPlot()
{
  setTitle(graph1->GetTitle());
  addGraph(new TGraph(*graph1));
  setAxisTitle(graph1->GetXaxis()->GetTitle(), graph1->GetYaxis()->GetTitle());
  addGraph(graph2);
}

GraphPostAnalysisPlot::~GraphPostAnalysisPlot()
{

}
