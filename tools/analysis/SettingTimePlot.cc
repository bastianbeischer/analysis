#include "SettingTimePlot.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "SimpleEvent.hh"

#include <TAxis.h>
#include <TPad.h>
#include <TH1.h>

#include <cmath>

#include <QDebug>

SettingTimePlot::SettingTimePlot(SettingType type, QDateTime first, QDateTime last)
  : AnalysisPlot(Enums::Testbeam)
  , H1DPlot()
  , m_type(type)
{
  switch (m_type) {
    case MagnetInstalled:
      setTitle("Magnet installed"); break;
    case Momentum:
      setTitle("Momentum"); break;
    case AbsMomentum:
      setTitle("Absolute momentum"); break;
    case Polarity:
      setTitle("Polarity"); break;
    default:
      setTitle("unknown");
  }
  setAxisTitle("time", title());

  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  int nBins = (t2 - t1) / 60;
  
  TH1D* histogram = 0;
  
  histogram = new TH1D(qPrintable(title()), "", nBins, t1, t2);
  addHistogram(histogram, H1DPlot::P);
  setDrawOption(H1DPlot::P);
  
  m_normalizationHistogram = new TH1D(qPrintable(title() + "normalization"), "", nBins, t1, t2);
}

SettingTimePlot::~SettingTimePlot()
{
  delete m_normalizationHistogram;
}

void SettingTimePlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  //get settings if present
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  if (!settings || settings->situation() != Enums::Testbeam11)
    return;

  double value = 0;
  switch (m_type) {
    case MagnetInstalled:
      value = settings->magnet(); break;
    case Momentum:
      value = settings->momentum() * settings->polarity(); break;
    case AbsMomentum:
      value = settings->momentum(); break;
    case Polarity:
      value = settings->polarity(); break;
    default:
      value = 0; break;
  }

  double t = event->time();
  histogram()->Fill(t, value);
  m_normalizationHistogram->Fill(t);
}

void SettingTimePlot::finalize()
{
  //histogram()->Divide(m_normalizationHistogram); //Cannot be used due to a ROOT bug leading to a rebin of the x axis.
  for (int bin = 1; bin <= xAxis()->GetNbins(); ++bin) {
    double n = m_normalizationHistogram->GetBinContent(bin);
    if (n > 0)
      histogram()->SetBinContent(bin, histogram()->GetBinContent(bin) / n);
  }
}

void SettingTimePlot::draw(TCanvas* canvas)
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
