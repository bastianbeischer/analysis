#include "H1DPlot.hh"

#include <TH1.h>
#include <THStack.h>
#include <TList.h>
#include <TCanvas.h>
#include <TPad.h>

H1DPlot::H1DPlot(Topic topic)
  : AnalysisPlot(topic)
  , m_histograms(new THStack())
  , m_xAxisTitle("")
  , m_yAxisTitle("")
{}

H1DPlot::~H1DPlot()
{
  for (int i = 0; i < numberOfHistograms(); ++i)
    delete histogram(i);
  delete m_histograms;
}

void H1DPlot::draw(TCanvas* canvas) const
{
  canvas->cd();
  m_histograms->Draw();
  m_histograms->GetXaxis()->SetTitle(qPrintable(m_xAxisTitle));
  m_histograms->GetYaxis()->SetTitle(qPrintable(m_yAxisTitle));
  gPad->Modified();
  gPad->Update();
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

void H1DPlot::setAxisTitles(const QString& xTitle, const QString& yTitle)
{
  m_xAxisTitle = xTitle;
  m_yAxisTitle = yTitle;
}
