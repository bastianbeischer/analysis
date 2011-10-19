#include "MeasurementTimePlot.hh"
#include "SimpleEvent.hh"
#include "Settings.hh"
#include "SettingsManager.hh"

#include <TH1.h>
#include <TAxis.h>
#include <TPad.h>
#include <TLatex.h>

#include <cmath>

#include <QDebug>

MeasurementTimePlot::MeasurementTimePlot(const QDateTime& first, const QDateTime& last)
  : AnalysisPlot(Enums::MomentumReconstruction)
  , H1DPlot()
{
  setTitle("measurement time");
  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  const int maxDuration = t2 - t1;
  double binWidth = 30.;
  if (maxDuration < 10000.)
    binWidth = 10.;
  const int nBins = int(maxDuration / binWidth) + 1;
  TH1D* histogram = new TH1D("measurement time", "", nBins, t1, t1 + nBins * binWidth);
  histogram->GetXaxis()->SetTitle("time");
  histogram->GetYaxis()->SetTitle("events");

  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  histogram->SetMarkerSize(0.5);
  addHistogram(histogram, H1DPlot::P);

  addLatex(RootPlot::newLatex(.2, .85));
  histogram->GetXaxis()->SetLabelOffset(0.01);
  histogram->GetXaxis()->SetTitleOffset(1.2);
  histogram->GetYaxis()->SetTitleOffset(1.2);
}

MeasurementTimePlot::~MeasurementTimePlot()
{
}

void MeasurementTimePlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  if (!event->contentType() == SimpleEvent::MonteCarlo && (event->time() < histogram()->GetXaxis()->GetXmin() || event->time() > histogram()->GetXaxis()->GetXmax()))
    qDebug() << "Eventtime is not between first and last" << event->time();
  else
    histogram()->Fill(event->time());
}

void MeasurementTimePlot::update()
{
  latex()->SetTitle(qPrintable(QString("bin width = %1 s").arg(histogram()->GetBinWidth(1))));
}

void MeasurementTimePlot::draw(TCanvas* canvas)
{
  if (m_drawn) {
    H1DPlot::draw(canvas);
  } else {
    H1DPlot::draw(canvas);
    //TODO check this, especially for older data !!!
    xAxis()->SetTimeOffset(0, "gmt");
    xAxis()->SetTimeDisplay(1);
    xAxis()->SetTimeFormat("%H:%M");
    gPad->Modified();
    gPad->Update();
  }
}

