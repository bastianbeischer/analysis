#include "EfficiencyPlot.hh"
#include "LogLikelihoodProjectionPlot.hh"

#include <TH1.h>

#include <QDebug>

EfficiencyPlot::EfficiencyPlot(LogLikelihoodProjectionPlot* projection)
  : PostAnalysisPlot()
  , H1DPlot()
{
  QString title = projection->title();
  title.replace("log likelihood histogram", "efficiency");
  setTitle(title);

  TAxis* xAxis = projection->signalProjection()->GetXaxis();
  int nBins = xAxis->GetNbins();
  const double* signal = projection->signalProjection()->GetIntegral();
  QVector<double> binning(nBins + 1);
  for (int bin = 0; bin <= nBins; ++bin)
    binning[bin] = xAxis->GetBinLowEdge(bin + 1);
  TH1D* efficiencyHistogram = new TH1D(qPrintable(title), "", nBins, binning.constData());
  for (int bin = 1; bin <= nBins; ++bin) {
    efficiencyHistogram->SetBinContent(bin, signal[bin]);
  }
  addHistogram(efficiencyHistogram);
  setAxisTitle(xAxis->GetTitle(), "efficiency");
}

EfficiencyPlot::~EfficiencyPlot()
{
}
