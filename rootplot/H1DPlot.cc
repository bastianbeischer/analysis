#include "H1DPlot.hh"

#include <TH1.h>
#include <TList.h>
#include <TCanvas.h>
#include <TPad.h>

#include <QDebug>

H1DPlot::H1DPlot()
  : RootPlot()
  , m_histograms()
{}

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
      h->Draw("SAME");
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

