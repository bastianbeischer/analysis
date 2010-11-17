#include "SignalHeightPlot.hh"

#include <TH1D.h>

#include "Hit.hh"

SignalHeightPlot::SignalHeightPlot(AnalysisPlot::Topic topic, unsigned short id) :
  AnalysisPlot(topic),
  H1DPlot(),
  m_id(id)
{
  int maximum = 0, bins = 0;
  if (topic == AnalysisPlot::SignalHeightUpperTracker || topic == AnalysisPlot::SignalHeightLowerTracker) {
    maximum = 4096;
    bins = 1024;
  }
  else if (topic == AnalysisPlot::SignalHeightTRD) {
    maximum = 1500;
    bins = 1500;
  }

  setTitle(QString("signal height 0x%1").arg(m_id,0,16));
  TH1D* histogram = new TH1D(qPrintable(title()), "", bins, 0, maximum);
  addHistogram(histogram);
}

SignalHeightPlot::~SignalHeightPlot()
{
}

void SignalHeightPlot::processEvent(const QVector<Hit*>& hits, Track* /*track*/, SimpleEvent* /*event*/)
{
  int nTrackerHits = 0;
  foreach(Hit* hit, hits)
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
    return;

  foreach(Hit* hit, hits) {
    if (hit->detId() - hit->channel() == m_id)
      histogram(0)->Fill(hit->signalHeight());
  }
}

void SignalHeightPlot::finalize()
{
}
