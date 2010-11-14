#include "H2DPlot.hh"

#include <TH2.h>
#include <THStack.h>
#include <TList.h>
#include <TCanvas.h>

H2DPlot::H2DPlot(Topic topic)
  : AnalysisPlot(topic)
  , m_histograms(new THStack)
{}

H2DPlot::~H2DPlot()
{
  delete m_histograms;
}
    
void H2DPlot::draw(TCanvas* canvas) const
{
  canvas->cd();
  m_histograms->Draw("COLZ");
}

void H2DPlot::clear()
{
  for (int i = 0; i < numberOfHistograms(); ++i)
    clear(i);
}

void H2DPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  histogram(i)->Clear();
}

int H2DPlot::numberOfHistograms()
{
  return m_histograms->GetHists()->GetSize();
}

TH2D* H2DPlot::histogram(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  return static_cast<TH2D*>(m_histograms->GetHists()->At(i));
}

void H2DPlot::addHistogram(TH2D* h)
{
  m_histograms->Add(h);
}
