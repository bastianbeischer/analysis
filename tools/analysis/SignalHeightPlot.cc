#include "SignalHeightPlot.hh"

#include <TH1D.h>

#include "Track.hh"
#include "TrackInformation.hh"
#include "Hit.hh"

SignalHeightPlot::SignalHeightPlot(AnalysisPlot::Topic topic, unsigned short id) :
  AnalysisPlot(topic),
  H1DPlot(),
  m_id(id)
{
  int maximum = 0, bins = 0;
  if (topic == AnalysisPlot::SignalHeightTracker) {
    maximum = 4096;
    bins = 100;
  }
  else if (topic == AnalysisPlot::SignalHeightTRD) {
    maximum = 1500;
    bins = 1500;
  }

  setTitle(QString("signal height 0x%1").arg(m_id,0,16));
  TH1D* histogram = new TH1D(qPrintable(title()), "", bins, 0, maximum);
  histogram->GetXaxis()->SetTitle("signal height / adc counts");
  histogram->GetYaxis()->SetTitle("entries");
  addHistogram(histogram);
}

SignalHeightPlot::~SignalHeightPlot()
{
}

void SignalHeightPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  if(!track)
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->detId() - hit->channel() == m_id)
      histogram()->Fill(hit->signalHeight());
  }
}
