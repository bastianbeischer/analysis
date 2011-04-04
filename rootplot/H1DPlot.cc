#include "H1DPlot.hh"

#include <TH1.h>
#include <TList.h>
#include <TCanvas.h>
#include <TPad.h>
#include <THStack.h>

const QVector<RootPlot::DrawOption> H1DPlot::s_drawOptions = QVector<DrawOption>()
  << BLANK << NOSTACK;

H1DPlot::H1DPlot()
  : RootPlot()
  , m_stack(new THStack)
  , m_histograms()
  , m_xAxisTitle()
  , m_yAxisTitle()
{
  m_drawOption = NOSTACK;
  m_type = RootPlot::H1DPlot;
}

H1DPlot::~H1DPlot()
{
  delete m_stack;
  qDeleteAll(m_histograms);
}

void H1DPlot::draw(TCanvas* canvas)
{
  if (!m_histograms.size())
    return;
  canvas->cd();
  foreach (TH1D* h, m_histograms)
    if (h == m_histograms.first()) {
      h->Draw();
    } else {
      if (m_histograms.first()->GetEntries() > 0)
        h->Draw("SAME");
      else
        h->Draw();
    }
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
  foreach (TH1D* h, m_histograms)
    h->Clear();
}

void H1DPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  m_histograms[i]->Clear();
}

int H1DPlot::numberOfHistograms()
{
  return m_histograms.size();
}

TH1D* H1DPlot::histogram(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  return m_histograms[i];
}

void H1DPlot::addHistogram(TH1D* h, DrawOption option)
{
  m_histograms.append(h);
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
