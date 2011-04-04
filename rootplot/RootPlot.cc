#include "RootPlot.hh"

#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TF1.h>

RootPlot::RootPlot()
  : m_title(QString())
  , m_drawOption(UndefinedDrawOption)
  , m_type(Undefined)
  , m_latex(QVector<TLatex*>())
  , m_legend(QVector<TLegend*>())
  , m_function(QVector<TF1*>())
{
}

RootPlot::~RootPlot()
{
  qDeleteAll(m_latex);
  qDeleteAll(m_legend);
  qDeleteAll(m_function);
}
  
TLatex* RootPlot::newLatex(double rx, double ry)
{
  TLatex* latex = new TLatex(rx, ry, 0);
  latex->SetNDC();
  latex->SetTextAlign(13);
  latex->SetTextFont(82);
  latex->SetTextSize(0.03);
  return latex;
}

void RootPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  foreach(TLatex* latex, m_latex)
    latex->Draw("SAME");
  foreach(TLegend* legend, m_legend)
    legend->Draw("SAME");
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

QString RootPlot::drawOption(DrawOption option)
{
  switch (option) {
    case COLZ: return "COLZ";
    case CONT4Z: return "CONT4 Z";
    case LEGO: return "LEGO";
    case LEGO2: return "LEGO2";
    case LEGOCOLZ: return "LEGO COL Z";
    case SURF1: return "SURF1";
    case COLZTEXT: return "COL Z TEXT";
    default: return QString();
  }
  return QString();
}
  
RootPlot::DrawOption RootPlot::drawOption()
{
  return m_drawOption;
}
  
void RootPlot::setDrawOption(DrawOption option)
{
  m_drawOption = option;
}
