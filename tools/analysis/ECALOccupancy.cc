#include "ECALOccupancy.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ECALHit.hh"

#include <TH2.h>
#include <TAxis.h>

ECALOccupancy::ECALOccupancy()
  : AnalysisPlot(Enums::Occupancy)
  , H2DPlot()
  , m_zPosition(-820)
  , m_ecalSideWidth(384)
{
  setTitle("ecal occupancy");

  QVector<int> ids;
  for (int i = 0; i < 12; ++i)
    ids << (0x7400 | i);
  for (int i = 0; i < 12; ++i)
    ids << (0x7500 | i);
  int color = 0;
  foreach (int id, ids) {
    QString histogramTitle = title() + " 0x" + QString::number(id, 16);
    TH2D* histogram = new TH2D(qPrintable(histogramTitle), "",
      240, -0.6 * m_ecalSideWidth, +0.6 * m_ecalSideWidth,
      240, -0.6 * m_ecalSideWidth, +0.6 * m_ecalSideWidth);
    histogram->SetMarkerColor(RootStyle::rootColor(color % 6) == 5 ? 28 : RootStyle::rootColor(color % 6));
    histogram->SetMarkerSize(histogram->GetMarkerSize() * 0.5);
    m_histograms.insert(id, histogram);
    addHistogram(histogram, RootPlot::SCAT);
    ++color;
  }

  setDrawOption(RootPlot::SCAT);
  setAxisTitle("x / mm", "y / mm", "");
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good);
}

ECALOccupancy::~ECALOccupancy()
{}

void ECALOccupancy::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  const SimpleEvent* simpleEvent = event->simpleEvent();
  std::vector<ECALHit*>::const_iterator it = simpleEvent->ecalHits().begin();
  std::vector<ECALHit*>::const_iterator end = simpleEvent->ecalHits().end();
  for (; it != end; ++it) {
    if (100 < (*it)->signalHeight()) {
      int id = (*it)->detId();
      QMap<unsigned short, TH2D*>::Iterator histogramIterator = m_histograms.find(id);
      if (histogramIterator != m_histograms.end()) {
        histogramIterator.value()->Fill(track->x(m_zPosition), track->y(m_zPosition));
      }
    }
  }
}
