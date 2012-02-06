#include "LogLikelihoodPlot.hh"
#include "PostAnalysisCanvas.hh"

#include <TAxis.h>
#include <TH2.h>

#include <QDebug>

LogLikelihoodPlot::LogLikelihoodPlot(PostAnalysisCanvas* signalHistogram, PostAnalysisCanvas* backgroundHistogram)
  : PostAnalysisPlot()
  , H2DPlot()
  , m_signalHistogram(static_cast<TH2D*>(signalHistogram->histograms2D().at(0)->Clone()))
  , m_backgroundHistogram(static_cast<TH2D*>(backgroundHistogram->histograms2D().at(0)->Clone()))
{
  QString signalLabel = m_signalHistogram->GetName();
  signalLabel.remove("log likelihood histogram ");
  signalLabel.remove(" signal");
  setTitle("log likelihood histogram " + signalLabel);
  m_signalHistogram->SetMarkerColor(kRed);
  addHistogram(m_signalHistogram, RootPlot::SCAT);
  m_backgroundHistogram->SetMarkerColor(kBlue);
  addHistogram(m_backgroundHistogram, RootPlot::SCAT);
  setDrawOption(RootPlot::SCAT);
  setAxisTitle(m_signalHistogram->GetXaxis()->GetTitle(), m_signalHistogram->GetYaxis()->GetTitle(), "");
}

LogLikelihoodPlot::~LogLikelihoodPlot()
{
}

int LogLikelihoodPlot::numberOfBins() const
{
  return m_signalHistogram->GetXaxis()->GetNbins();
}

const TH2D* LogLikelihoodPlot::signalHistogram() const
{
  return m_signalHistogram;
}

const TH2D* LogLikelihoodPlot::backgroundHistogram() const
{
  return m_backgroundHistogram;
}
