#include "H1DPlot.hh"

#include <TH1.h>
#include <THStack.h>
#include <TList.h>
#include <TCanvas.h>

H1DPlot::H1DPlot(Topic topic)
  : AnalysisPlot(topic)
  , m_histograms(new THStack())
{}

H1DPlot::~H1DPlot()
{
  delete m_histograms;
}

void H1DPlot::draw(TCanvas* canvas) const
{
  canvas->cd();
  m_histograms->Draw();
}

void H1DPlot::clear()
{
  for (int i = 0; i < numberOfHistograms(); ++i)
    clear(i);
}

void H1DPlot::clear(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  histogram(i)->Clear();
}

int H1DPlot::numberOfHistograms()
{
  return m_histograms->GetHists()->GetSize();
}

TH1D* H1DPlot::histogram(int i)
{
  Q_ASSERT(0 <= i && i < numberOfHistograms());
  return static_cast<TH1D*>(m_histograms->GetHists()->At(i));
}

void H1DPlot::addHistogram(TH1D* h)
{
  m_histograms->Add(h);
}

