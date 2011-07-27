#include "PostAnalysisGraphPlot.hh"

#include <TGraph.h>
#include <TAxis.h>

PostAnalysisGraphPlot::PostAnalysisGraphPlot(TGraph* graph)
  : PostAnalysisPlot()
  , GraphPlot()
{
  init(QVector<TGraph*>() << graph);
}

PostAnalysisGraphPlot::PostAnalysisGraphPlot(const QVector<TGraph*>& graphs)
  : PostAnalysisPlot()
  , GraphPlot()
{
  init(graphs);
}

PostAnalysisGraphPlot::~PostAnalysisGraphPlot()
{
}

void PostAnalysisGraphPlot::init(const QVector<TGraph*>& graphs)
{
  Q_ASSERT(graphs.count());
  setTitle(graphs[0]->GetTitle());
  setAxisTitle(graphs[0]->GetXaxis()->GetTitle(), graphs[0]->GetYaxis()->GetTitle());
  foreach (TGraph* graph, graphs)
    addGraph(new TGraph(*graph));
}
