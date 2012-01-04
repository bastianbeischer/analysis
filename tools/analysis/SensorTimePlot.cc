#include "SensorTimePlot.hh"
#include "Hit.hh"
#include "SensorTypes.hh"
#include "SimpleEvent.hh"

#include <TAxis.h>
#include <TPad.h>
#include <TH1.h>

#include <cmath>

SensorTimePlot::SensorTimePlot(SensorTypes::Type type, QDateTime first, QDateTime last)
  : AnalysisPlot(Enums::SlowControl)
  , H1DPlot()
  , m_type(type)
{
  setTitle(QString::fromStdString(SensorTypes::convertToString(m_type)));
  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  int nBins = (t2 - t1) / 60;

  TH1D* histogram = 0;

  histogram = new TH1D(qPrintable(title()), "", nBins, t1, t2);
  histogram->SetMarkerSize(0.5);
  addHistogram(histogram, H1DPlot::P);
  setDrawOption(H1DPlot::P);

  m_normalizationHistogram = new TH1D(qPrintable(title() + "normalization"), "", nBins, t1, t2);
}

SensorTimePlot::~SensorTimePlot()
{
  delete m_normalizationHistogram;
}

void SensorTimePlot::processEvent(const AnalyzedEvent* event)
{
  double value = event->simpleEvent()->sensorData(m_type);
  if (!std::isnan(value)) {
    double t = event->simpleEvent()->time();
    histogram()->Fill(t, value);
    m_normalizationHistogram->Fill(t);
  }
}

void SensorTimePlot::finalize()
{
  //histogram()->Divide(m_normalizationHistogram); //Cannot be used due to a ROOT bug leading to a rebin of the x axis.
  for (int bin = 1; bin <= xAxis()->GetNbins(); ++bin) {
    double n = m_normalizationHistogram->GetBinContent(bin);
    if (n > 0)
      histogram()->SetBinContent(bin, histogram()->GetBinContent(bin) / n);
  }
}

void SensorTimePlot::draw(TCanvas* canvas)
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
