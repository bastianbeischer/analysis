#include "H2DPlot.hh"

#include <TH2.h>
#include <TList.h>
#include <TCanvas.h>

H2DPlot::H2DPlot()
  : RootPlot()
  , m_histogram()
{}

H2DPlot::~H2DPlot()
{
  delete m_histogram;
}
    
void H2DPlot::draw(TCanvas* canvas)
{
  if (!m_histogram)
    return;
  canvas->cd();
  m_histogram->Draw("COLZ");
  RootPlot::draw(canvas);
}

void H2DPlot::unzoom()
{
  if (!m_histogram)
    return;
  m_histogram->GetXaxis()->UnZoom();
  m_histogram->GetYaxis()->UnZoom();
  m_histogram->GetZaxis()->UnZoom();
}

void H2DPlot::clear()
{
  if (!m_histogram)
    return;
  m_histogram->Clear();
}

TH2D* H2DPlot::histogram()
{
  return m_histogram;
}

void H2DPlot::setHistogram(TH2D* h)
{
  if (m_histogram)
    delete m_histogram;
  m_histogram = h;
}
