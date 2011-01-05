#include "GraphPlot.hh"

#include <TCanvas.h>
#include <TGraph.h>
#include <TList.h>
#include <TMultiGraph.h>

GraphPlot::GraphPlot() :
  m_multiGraph(new TMultiGraph)
{
}

GraphPlot::~GraphPlot()
{
  delete m_multiGraph; // deletes all others...
}

void GraphPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  m_multiGraph->Draw("A");
  RootPlot::draw(canvas);
}

void GraphPlot::clear()
{
  for (int i = 0; i < numberOfGraphs(); i++)
    clear(i);
}

void GraphPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i < numberOfGraphs());
  TList* graphs = m_multiGraph->GetListOfGraphs();
  TGraph* graph = static_cast<TGraph*>(graphs->At(i));
  graph->Clear();
}

void GraphPlot::addGraph(TGraph* g, const char* options)
{
  m_multiGraph->Add(g, options);
}

void GraphPlot::setMultiGraphTitle(const QString& title)
{
  setTitle(title);
  m_multiGraph->SetTitle(qPrintable(title));
}

int GraphPlot::numberOfGraphs() const
{
  TList* graphs = m_multiGraph->GetListOfGraphs();
  return graphs->GetEntries();
}

TGraph* GraphPlot::graph(int i)
{
  Q_ASSERT(0 <= i && i < numberOfGraphs());
  TList* graphs = m_multiGraph->GetListOfGraphs();
  return static_cast<TGraph*>(graphs->At(i));
}

TMultiGraph* GraphPlot::multiGraph()
{
  return m_multiGraph;
}
