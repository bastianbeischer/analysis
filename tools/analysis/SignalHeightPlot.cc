#include "SignalHeightPlot.hh"

#include <TH1D.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Hit.hh"

SignalHeightPlot::SignalHeightPlot(Enums::AnalysisTopic topic, unsigned short id) :
  AnalysisPlot(topic),
  H1DPlot(),
  m_id(id)
{
  int maximum = 0, bins = 0;
  if (topic == Enums::SignalHeightTracker) {
    maximum = 4096;
    bins = 100;
  }
  else if (topic == Enums::SignalHeightTRD) {
    maximum = 1500;
    bins = 1500;
  }

  setTitle(QString("signal height 0x%1").arg(m_id,0,16));
  TH1D* histogram = new TH1D(qPrintable(title()), "", bins, 0, maximum);
  setAxisTitle("signal height / adc counts", "");
  addHistogram(histogram);

  addRequiredEventFlags(Enums::TrackGood | Enums::AllTrackerLayers);
}

SignalHeightPlot::~SignalHeightPlot()
{
}

void SignalHeightPlot::processEvent(const AnalyzedEvent* event)
{
  const QVector<Hit*>::const_iterator endIt = event->clusters().end();
  for (QVector<Hit*>::const_iterator it = event->clusters().begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->elementId() == m_id)
      histogram()->Fill(hit->signalHeight());
  }
}
