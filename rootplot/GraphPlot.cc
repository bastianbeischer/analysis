#include "GraphPlot.hh"

#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TList.h>
#include <TMultiGraph.h>

const QVector<RootPlot::DrawOption> GraphPlot::s_drawOptions = QVector<DrawOption>();

GraphPlot::GraphPlot()
  : m_multiGraph(new TMultiGraph)
  , m_xAxisTitle()
  , m_yAxisTitle()
{
  m_type = RootPlot::GraphPlot;
}

GraphPlot::~GraphPlot()
{
  delete m_multiGraph;
}

void GraphPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  canvas->Clear();
  if (!m_drawn)
    m_multiGraph->SetTitle(qPrintable(";" + m_xAxisTitle + ";" + m_yAxisTitle));
  m_multiGraph->Draw("A");
  m_drawn = true;
  RootPlot::draw(canvas);
}

void GraphPlot::unzoom()
{
  if (m_drawn) {
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

const QVector<RootPlot::DrawOption>& GraphPlot::drawOptions()
{
  return s_drawOptions;
}

void GraphPlot::setAxisTitle(const QString& x, const QString& y)
{
  m_xAxisTitle = x;
  m_yAxisTitle = y;
}
