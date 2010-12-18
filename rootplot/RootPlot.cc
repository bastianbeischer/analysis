#include "RootPlot.hh"

#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TF1.h>

RootPlot::RootPlot()
{}

RootPlot::~RootPlot()
{
  qDeleteAll(m_latex);
  qDeleteAll(m_legend);
  qDeleteAll(m_function);
}
  
void RootPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  foreach(TLatex* latex, m_latex)
    latex->Draw();
  foreach(TLegend* legend, m_legend)
    legend->Draw();
  foreach(TF1* function, m_function)
    function->Draw("SAME");
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

void RootPlot::addFunction(TF1* function)
{
  Q_ASSERT(function);
  m_function.append(function);
}

TF1* RootPlot::function(int i)
{
  Q_ASSERT(0 <= i && i < m_function.size());
  return m_function[i];
}
