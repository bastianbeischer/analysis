#include "H1DPlot.hh"

#include <TH1.h>
#include <TList.h>
#include <TCanvas.h>
#include <TPad.h>
#include <THStack.h>

#include <QDebug>

const QVector<RootPlot::DrawOption> H1DPlot::s_drawOptions = QVector<DrawOption>()
  << NOSTACK << BLANK;

H1DPlot::H1DPlot()
  : RootPlot()
  , m_stack(new THStack)
  , m_xAxisTitle()
  , m_yAxisTitle()
{
  m_drawOption = NOSTACK;
  m_type = RootPlot::H1DPlot;
}

H1DPlot::~H1DPlot()
{
  for (int i = 0; i < numberOfHistograms(); ++i)
    delete histogram(i);
  delete m_stack;
}

void H1DPlot::draw(TCanvas* canvas)
{
  if (!numberOfHistograms())
    return;
  canvas->cd();
  if (!m_drawn)
    m_stack->SetTitle(qPrintable(";" + m_xAxisTitle + ";" + m_yAxisTitle));
  m_stack->Draw(qPrintable(drawOption(m_drawOption)));
  m_drawn = true;
  RootPlot::draw(canvas);
}

void H1DPlot::unzoom()
{
  if (m_drawn) {
    m_stack->GetXaxis()->UnZoom();
    m_stack->GetYaxis()->UnZoom();
  }
}

void H1DPlot::clear()
{
  for (int i = 0; i < numberOfHistograms(); ++i)
    histogram(i)->Clear();
}

void H1DPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  histogram(i)->Clear();
}

int H1DPlot::numberOfHistograms()
{
  return m_stack->GetHists() ? m_stack->GetHists()->GetSize() : 0;
}

TH1D* H1DPlot::histogram(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  return static_cast<TH1D*>(m_stack->GetHists()->At(i));
}

void H1DPlot::addHistogram(TH1D* h, DrawOption option)
{
  m_stack->Add(h, qPrintable(drawOption(option)));
}

const QVector<RootPlot::DrawOption>& H1DPlot::drawOptions()
{
  return s_drawOptions;
}
  
void H1DPlot::setAxisTitle(const QString& x, const QString& y)
{
  m_xAxisTitle = x;
  m_yAxisTitle = y;
}
