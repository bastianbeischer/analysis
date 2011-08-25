#include "GraphPlot.hh"

#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TList.h>
#include <TMultiGraph.h>

const QVector<RootPlot::DrawOption> GraphPlot::s_drawOptions = QVector<DrawOption>()
  << P << AP << AC << ALP << ACP << ALX << LX << LP;

GraphPlot::GraphPlot()
  : RootPlot()
  , m_graphs()
  , m_xAxis(0)
  , m_yAxis(0)
  , m_xAxisTitle()
  , m_yAxisTitle()
  , m_drawOptions()
{
  m_drawOption = ALP;
  m_type = RootPlot::GraphPlot;
}

GraphPlot::~GraphPlot()
{
  foreach (TGraph* g, m_graphs)
    delete g;
  m_graphs.clear();
}

void GraphPlot::draw(TCanvas* canvas)
{
  if (!numberOfGraphs())
    return;
  canvas->cd();
  graph()->SetTitle(qPrintable(";" + m_xAxisTitle + ";" + m_yAxisTitle));
  graph()->Draw(qPrintable(drawOption(m_drawOption)));
  for (int i = 1; i < numberOfGraphs(); ++i)
    graph(i)->Draw(qPrintable("SAME" + drawOption(m_drawOptions[i])));
  m_drawn = true;
  RootPlot::draw(canvas);
}

void GraphPlot::unzoom()
{
  if (m_drawn) {
    xAxis()->UnZoom();
    yAxis()->UnZoom();
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
  m_graphs[i]->Clear();
}

int GraphPlot::numberOfGraphs() const
{
  return m_graphs.count();
}

void GraphPlot::addGraph(TGraph* g, DrawOption option)
{
  m_graphs.append(g);
  m_drawOptions.append(option);
  if (!m_xAxis && !m_yAxis) {
    m_xAxis = g->GetXaxis();
    m_yAxis = g->GetYaxis();
  }
}

void GraphPlot::removeGraph(int i)
{
  Q_ASSERT(0 <= i && i < numberOfGraphs());
  delete m_graphs[i];
  m_graphs[i] = 0;
  m_graphs.remove(i);
  m_drawOptions.remove(i);
}

TGraph* GraphPlot::graph(int i)
{
  Q_ASSERT(0 <= i && i < numberOfGraphs());
  return m_graphs[i];
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
  
TAxis* GraphPlot::xAxis()
{
  Q_ASSERT(m_xAxis);
  return m_xAxis;
}

TAxis* GraphPlot::yAxis()
{
  Q_ASSERT(m_yAxis);
  return m_yAxis;
}
