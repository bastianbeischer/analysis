#include "TriggerRateTimePlot.hh"
#include "SimpleEvent.hh"

#include <TH1.h>
#include <TAxis.h>
#include <TPad.h>
#include <THStack.h>

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
  int nBins = (t2 - t1) / 60;
  TH1D* histogram = new TH1D("trigger rate", "", nBins, t1, t2);
  setAxisTitle("time", "trigger rate / Hz");
  histogram->SetMarkerSize(0.5);
  addHistogram(histogram, H1DPlot::P);
  setDrawOption(H1DPlot::P);
}

TriggerRateTimePlot::~TriggerRateTimePlot()
{}

void TriggerRateTimePlot::processEvent(const QVector<Hit*>&, Particle*, SimpleEvent* event)
{
  if (event->time() < xAxis()->GetXmin() || event->time() >= xAxis()->GetXmax())
    qDebug() << event->time();
  else
    histogram()->Fill(event->time());
}

void TriggerRateTimePlot::finalize()
{
  double binWidth = xAxis()->GetBinWidth(1);
  //histogram()->Scale(1./binWidth); //Cannot be used due to a ROOT bug leading to a rebin of the x axis.
  for (int bin = 1; bin <= xAxis()->GetNbins(); ++bin)
    histogram()->SetBinContent(bin, histogram()->GetBinContent(bin) / binWidth);
}

void TriggerRateTimePlot::draw(TCanvas* canvas)
{
  if (m_drawn) {
    H1DPlot::draw(canvas);
  } else {
    H1DPlot::draw(canvas);
    //TODO check this, especially for older data !!!
    xAxis()->SetTimeOffset(3600, "gmt"); //dont understand this, but works at testbeam
    xAxis()->SetTimeDisplay(1);
    xAxis()->SetTimeFormat("%d-%H:%M");
    gPad->Modified();
    gPad->Update();
  }
}
