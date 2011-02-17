#include "GraphPlot.hh"

#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TList.h>
#include <TMultiGraph.h>

GraphPlot::GraphPlot() :
  m_multiGraph(new TMultiGraph)
{
}

GraphPlot::~GraphPlot()
{
  delete m_multiGraph;
}

void GraphPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  canvas->Clear();
  m_multiGraph->Draw("A");
  RootPlot::draw(canvas);
}

void GraphPlot::unzoom()
{
  if (m_multiGraph->GetXaxis() && m_multiGraph->GetYaxis()) {
    m_multiGraph->GetXaxis()->UnZoom();
    m_multiGraph->GetYaxis()->UnZoom();
  }
}

void GraphPlot::clear()
{
  for (int i = 0; i < numberOfGraphs(); i++)
    clear(i);
}

void GraphPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i < numberOfGraphs());
  TGraph* graph = static_cast<TGraph*>(m_multiGraph->GetListOfGraphs()->At(i));
  graph->Clear();
}

void GraphPlot::addGraph(TGraph* g, const char* options)
{
  m_multiGraph->Add(g, options);
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
