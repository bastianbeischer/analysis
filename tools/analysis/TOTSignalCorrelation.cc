#include "TOTSignalCorrelation.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Constants.hh"
#include "ParticleInformation.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>

TOTSignalCorrelation::TOTSignalCorrelation(const QString& title, Hit::ModuleType type, TOTLayerPlot::Layer layer)
  : H2DPlot()
  , TOTLayerPlot(layer)
  , m_type(type)
{
  QString fullTitle = title;
  unsigned int nBinsX = 150;
  double xMin = 0;
  double xMax = 1e5;
  if (m_type == Hit::trd) {
    fullTitle+= " trd";
    nBinsX = 150;
    xMin = 0;
    xMax = 7e2;
  }
  if (m_type == Hit::tracker) {
    fullTitle+= " tracker";
    nBinsX = 150;
    xMin = 0;
    xMax = 7e4;
  }
  fullTitle+= layerName();
  setTitle(fullTitle);
  TH2D* histogram = new TH2D(qPrintable(fullTitle), "", nBinsX, xMin, xMax, 100, 0, 100);
  setAxisTitle("signal height / ADC counts", "mean time over threshold / ns", "");
  addHistogram(histogram);
}

TOTSignalCorrelation::~TOTSignalCorrelation()
{}

void TOTSignalCorrelation::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet))
    return;
  double totSum = 0.;
  int nTofHits = 0;
  const QVector<Hit*>& clusters = track->hits();
  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() == Hit::tof) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
      double z = tofCluster->position().z();
      if (qAbs(track->x(z) - tofCluster->position().x()) > 0.95 * Constants::tofBarWidth / 2.)
        continue;
      if (!checkLayer(z))
        continue;
      const std::vector<Hit*>& subHits = tofCluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(*it);
        totSum+= tofSipmHit->timeOverThreshold();
        ++nTofHits;
      }
    }
  }
  if (nTofHits > 0) {
    double nonTofSignal = sumOfNonTOFSignalHeights(event->clusters());
    histogram()->Fill(nonTofSignal, totSum / nTofHits);
  }
}

double TOTSignalCorrelation::sumOfNonTOFSignalHeights(const QVector<Hit*>& clusters) {
  double sumSignal = 0;
  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() != Hit::tof && hit->type() == m_type) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      const std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        sumSignal+= (*it)->signalHeight();
      }
    }
  }
  return sumSignal;
}

void TOTSignalCorrelation::finalize()
{
  setDrawOption(CONT4Z);
}
