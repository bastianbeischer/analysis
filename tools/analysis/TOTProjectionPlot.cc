#include "TOTProjectionPlot.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Constants.hh"
#include "ParticleInformation.hh"

#include <TH1D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>

TOTProjectionPlot::TOTProjectionPlot(const QString& title, TOTLayerPlot::Layer layer)
  : H1DPlot()
  , TOTLayerPlot(layer)
{
  QString fullTitle = title + " " + layerName();
  setTitle(fullTitle);
  TH1D* histogram = new TH1D(qPrintable(fullTitle), "", 150, 0, 100);
  setAxisTitle("mean time over threshold / ns", "");
  addHistogram(histogram);
}

TOTProjectionPlot::~TOTProjectionPlot()
{
}

void TOTProjectionPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet))
    return;
  const QVector<Hit*>& clusters = track->hits();
  double totSum = 0.;
  int nTofHits = 0;
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
    histogram()->Fill(totSum / nTofHits);
  }
} 
