#include "H2DPlot.hh"

#include "TH2.h"
#include "TCanvas.h"

H2DPlot::H2DPlot(Topic topic, const QString& title, int nBinsX, double xMin, double xMax, int nBinsY, double yMin, double yMax)
  : AnalysisPlot(topic, title)
  , m_histogram(new TH2D(qPrintable(title), qPrintable(title), nBinsX, xMin, xMax, nBinsY, yMin, yMax))
{}

H2DPlot::~H2DPlot()
{
  delete m_histogram;
}
    
void H2DPlot::draw(TCanvas* canvas) const
{
  canvas->cd();
  m_histogram->Draw();
}

void H2DPlot::clear()
{
  m_histogram->Clear();
}
