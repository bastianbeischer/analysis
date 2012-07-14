#include "ExternalTrackerOccupancy.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"
#include "AdditionalHit.hh"

#include <TH2.h>
#include <TAxis.h>

ExternalTrackerOccupancy::ExternalTrackerOccupancy(const QVector<int>& ids, double zPosition)
  : AnalysisPlot(Enums::Testbeam)
  , H2DPlot()
  , m_zPosition(zPosition)
{
  setTitle("external tracker occupancy " + QString::number(zPosition) + "mm");
  int color = 0;
  foreach (int id, ids) {
    QString histogramTitle = title() + " 0x" + QString::number(id, 16);
    TH2D* histogram = new TH2D(qPrintable(histogramTitle), "", 2000, -50., +50., 80, -40., +40.);
    histogram->SetMarkerColor(RootStyle::rootColor(color % 6) == 5 ? 28 : RootStyle::rootColor(color % 6));
    histogram->SetMarkerSize(0.2);
    m_histograms.insert(id, histogram);
    addHistogram(histogram, RootPlot::SCAT);
    ++color;
  }

  setDrawOption(RootPlot::SCAT);
  setAxisTitle("x / mm", "y / mm", "");
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good);
}

ExternalTrackerOccupancy::~ExternalTrackerOccupancy()
{}

void ExternalTrackerOccupancy::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  const SimpleEvent* simpleEvent = event->simpleEvent();
  std::vector<AdditionalHit*>::const_iterator end = simpleEvent->additionalHits().end();
  AdditionalHit* maximumHit = 0;
  for (std::vector<AdditionalHit*>::const_iterator it = simpleEvent->additionalHits().begin(); it != end; ++it) {
    if (m_histograms.find((*it)->detId()) == m_histograms.end())
      continue;
    if (!maximumHit || maximumHit->signalHeight() < (*it)->signalHeight())
      maximumHit = *it;
  }
  if (!maximumHit || maximumHit->signalHeight() < 100.)
    return;
  QMap<unsigned short, TH2D*>::Iterator histogramIterator = m_histograms.find(maximumHit->detId());
  if (histogramIterator == m_histograms.end())
    return;
  histogramIterator.value()->Fill(track->x(m_zPosition), track->y(m_zPosition));
}
