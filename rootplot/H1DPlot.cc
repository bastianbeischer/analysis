#include "H1DPlot.hh"

#include <TH1.h>
#include <TList.h>
#include <TCanvas.h>
#include <TPad.h>

const QVector<RootPlot::DrawOption> H1DPlot::s_drawOptions = QVector<DrawOption>();

H1DPlot::H1DPlot()
  : RootPlot()
  , m_histograms()
{
  m_type = RootPlot::H1DPlot;
}

H1DPlot::~H1DPlot()
{
  qDeleteAll(m_histograms);
}

void H1DPlot::draw(TCanvas* canvas)
{
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
  RootPlot::draw(canvas);
}

void H1DPlot::unzoom()
{
  if (m_histograms.size()) {
    m_histograms[0]->GetXaxis()->UnZoom();
    m_histograms[0]->GetYaxis()->UnZoom();
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

void H1DPlot::addHistogram(TH1D* h)
{
  m_histograms.append(h);
}

const QVector<RootPlot::DrawOption>& H1DPlot::drawOptions()
{
  return s_drawOptions;
}
