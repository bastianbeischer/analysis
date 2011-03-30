#include "TriggerRateTimePlot.hh"
#include "SimpleEvent.hh"

#include <TH1.h>
#include <TAxis.h>

#include <QDebug>

TriggerRateTimePlot::TriggerRateTimePlot(QDateTime first, QDateTime last)
  : AnalysisPlot(AnalysisPlot::SlowControl)
  , H1DPlot()
{
  setTitle("trigger rate");
  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  int nBins = qMin((t2 - t1) / 60, 1000);
  TH1D* histogram = new TH1D("trigger rate", "", nBins, t1, t2);
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
  histogram->GetXaxis()->SetTitle("time");
  addHistogram(histogram);
}

TriggerRateTimePlot::~TriggerRateTimePlot()
{}

void TriggerRateTimePlot::processEvent(const QVector<Hit*>&, Track*, SimpleEvent* event)
{
  if (event->time() < histogram()->GetXaxis()->GetXmin() || event->time() >= histogram()->GetXaxis()->GetXmax())
    qDebug() << event->time();
  else
    histogram()->Fill(event->time());
}

void TriggerRateTimePlot::finalize()
{
  double binWidth = histogram()->GetXaxis()->GetBinWidth(1);
  //histogram()->Scale(1./binWidth); //Cannot be used due to a ROOT bug leading to a rebin of the x axis.
  for (int bin = 1; bin <= histogram()->GetXaxis()->GetNbins(); ++bin)
    histogram()->SetBinContent(bin, histogram()->GetBinContent(bin) / binWidth);
  histogram()->GetYaxis()->SetTitle("trigger rate / Hz");
}
