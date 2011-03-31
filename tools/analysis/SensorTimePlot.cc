#include "SensorTimePlot.hh"
#include "Hit.hh"
#include "SensorTypes.hh"
#include "SimpleEvent.hh"

#include <TH2.h>
#include <TAxis.h>

#include <math.h>

#include <QDebug>

SensorTimePlot::SensorTimePlot(SensorTypes::Type type, QDateTime first, QDateTime last)
  : AnalysisPlot(AnalysisPlot::SlowControl)
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
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
  histogram->GetXaxis()->SetTitle("time");
  addHistogram(histogram);
  
  m_normalizationHistogram = new TH1D(qPrintable(title() + "normalization"), "", nBins, t1, t2);
}

SensorTimePlot::~SensorTimePlot()
{
  delete m_normalizationHistogram;
}

void SensorTimePlot::processEvent(const QVector<Hit*>&, Track*, SimpleEvent* event)
{
  double value = event->sensorData(m_type);
  if (!isnan(value)) {
    double t = event->time();
    histogram()->Fill(t, value);
    m_normalizationHistogram->Fill(t);
  }
}

void SensorTimePlot::finalize()
{
  //histogram()->Divide(m_normalizationHistogram); //Cannot be used due to a ROOT bug leading to a rebin of the x axis.
  for (int bin = 1; bin <= histogram()->GetXaxis()->GetNbins(); ++bin) {
    double n = m_normalizationHistogram->GetBinContent(bin);
    if (n > 0)
      histogram()->SetBinContent(bin, histogram()->GetBinContent(bin) / n);
  }
}