#include "RejectionPlot.hh"
#include "LogLikelihoodProjectionPlot.hh"

#include <TH1.h>

#include <QDebug>

RejectionPlot::RejectionPlot(LogLikelihoodProjectionPlot* projection)
  : PostAnalysisPlot()
  , H1DPlot()
{
  QString title = projection->title();
  title.replace("log likelihood histogram", "rejection");
  setTitle(title);

  TAxis* xAxis = projection->signalProjection()->GetXaxis();
  int nBins = xAxis->GetNbins();
  Q_ASSERT(nBins == projection->backgroundProjection()->GetXaxis()->GetNbins());
  const double* signal = projection->signalProjection()->GetIntegral();
  const double* background = projection->backgroundProjection()->GetIntegral();
  QVector<double> binning(nBins + 1);
  for (int bin = 0; bin <= nBins; ++bin)
    binning[bin] = xAxis->GetBinLowEdge(bin + 1);
  TH1D* rejectionHistogram = new TH1D(qPrintable(title), "", nBins, binning.constData());
  for (int bin = 1; bin <= nBins; ++bin) {
    if (!qIsNull(background[bin]))
      rejectionHistogram->SetBinContent(bin, 1. + signal[bin]/background[bin]);
  }
  addHistogram(rejectionHistogram);
  setAxisTitle(xAxis->GetTitle(), "rejection");
}

RejectionPlot::~RejectionPlot()
{
}
