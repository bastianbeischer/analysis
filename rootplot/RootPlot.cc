#include "RootPlot.hh"

#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>

RootPlot::RootPlot()
{}

RootPlot::~RootPlot()
{
  qDeleteAll(m_latex);
  qDeleteAll(m_legend);
}
  
void RootPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  foreach(TLatex* latex, m_latex)
    latex->Draw();
  foreach(TLegend* legend, m_legend)
    legend->Draw();
}

void RootPlot::addLatex(TLatex* latex)
{
  Q_ASSERT(latex);
  m_latex.append(latex);
}

TLatex* RootPlot::latex(int i)
{
  Q_ASSERT(0 <= i && i < m_latex.size());
  return m_latex[i];
}

void RootPlot::addLegend(TLegend* legend)
{
  Q_ASSERT(legend);
  m_legend.append(legend);
}

TLegend* RootPlot::legend(int i)
{
  Q_ASSERT(0 <= i && i < m_legend.size());
  return m_legend[i];
}
