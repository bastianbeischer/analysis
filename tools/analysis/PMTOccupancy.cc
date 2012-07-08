#include "PMTOccupancy.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"
#include "AdditionalHit.hh"

#include <TH2.h>
#include <TAxis.h>

PMTOccupancy::PMTOccupancy(int id, double zPosition)
  : AnalysisPlot(Enums::Occupancy)
  , H2DPlot()
  , m_id(id)
  , m_zPosition(zPosition)
{
  setTitle("ecal occupancy");

  QString histogramTitle = title() + " 0x" + QString::number(id, 16);
  TH2D* histogram = new TH2D(qPrintable(histogramTitle), "", 100, -1000, +1000, 100, -1000, +1000);
  addHistogram(histogram, RootPlot::SCAT);

  setDrawOption(RootPlot::SCAT);
  setAxisTitle("x / mm", "y / mm", "");
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good);
}

PMTOccupancy::~PMTOccupancy()
{}

void PMTOccupancy::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  const SimpleEvent* simpleEvent = event->simpleEvent();
  std::vector<AdditionalHit*>::const_iterator it = simpleEvent->additionalHits().begin();
  std::vector<AdditionalHit*>::const_iterator end = simpleEvent->additionalHits().end();
  for (; it != end; ++it) {
    int id = (*it)->detId();
    if (id == m_id) {
      Q_ASSERT((*it)->type() == AdditionalHit::PMT);
      if (100 < (*it)->signalHeight())
        histogram()->Fill(track->x(m_zPosition), track->y(m_zPosition));
    }
  }
}
