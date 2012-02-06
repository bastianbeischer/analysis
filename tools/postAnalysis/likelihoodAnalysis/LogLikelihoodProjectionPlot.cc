#include "LogLikelihoodProjectionPlot.hh"
#include "LogLikelihoodPlot.hh"

#include <TH2.h>

LogLikelihoodProjectionPlot::LogLikelihoodProjectionPlot(const LogLikelihoodPlot* plot, int bin, int numberOfBins)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_signalProjection(0)
  , m_backgroundProjection(0)
{
  const TH2D* signal = plot->signalHistogram();
  const TH2D* background = plot->backgroundHistogram();
  double lowerEdge = signal->GetXaxis()->GetBinLowEdge(bin);
  double upperEdge = signal->GetXaxis()->GetBinUpEdge(bin + numberOfBins - 1);
  QString title = QString("%1 %2-%3 GV").arg(plot->title())
    .arg(lowerEdge, 4, 'f', 2, '0').arg(upperEdge, 4, 'f', 2, '0');
  m_signalProjection = signal->ProjectionY(qPrintable(title + " signal"), bin, bin + numberOfBins - 1);
  m_signalProjection->SetLineColor(kRed);
  m_backgroundProjection = background->ProjectionY(qPrintable(title + " background"), bin, bin + numberOfBins - 1);
  m_backgroundProjection->SetLineColor(kBlue);

  addHistogram(m_signalProjection);
  addHistogram(m_backgroundProjection);
  setTitle(title);
  setAxisTitle(signal->GetYaxis()->GetTitle(), "");
}

LogLikelihoodProjectionPlot::~LogLikelihoodProjectionPlot()
{
}

TH1D* LogLikelihoodProjectionPlot::signalProjection() const
{
  return m_signalProjection;
}

TH1D* LogLikelihoodProjectionPlot::backgroundProjection() const
{
  return m_backgroundProjection;
}
