#include "H1DPlot.hh"

#include "TH1.h"
#include "TCanvas.h"

H1DPlot::H1DPlot(Topic topic, const QString& title, int nBins, double xMin, double xMax)
  : AnalysisPlot(topic, title)
  , m_histogram(new TH1D(qPrintable(title+QString::number(id())), "", nBins, xMin, xMax))
{}

H1DPlot::~H1DPlot()
{
  delete m_histogram;
}
    
void H1DPlot::draw(TCanvas* canvas) const
{
  canvas->cd();
  m_histogram->Draw();
}

void H1DPlot::clear()
{
  m_histogram->Clear();
}
