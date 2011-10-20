#include "TOTMomentumCorrelation.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"

#include <TH2D.h>
#include <TLatex.h>

#include <QDebug>

TOTMomentumCorrelation::TOTMomentumCorrelation(const QString& title, TOTLayerPlot::Layer layer)
  : H2DPlot()
  , TOTLayerPlot(layer)
{
  QString fullTitle = title + " " + layerName();
  setTitle(fullTitle);
  TH2D* histogram = new TH2D(qPrintable(fullTitle), "", 100, 0, 10, 150, 0, 100);
  setAxisTitle("rigidity / GV", "mean time over threshold / ns", "");
  addHistogram(histogram);
}

TOTMomentumCorrelation::~TOTMomentumCorrelation()
{}

void TOTMomentumCorrelation::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  if (!track || !track->fitGood())
    return;
  const QVector<Hit*>& clusters = track->hits();
  ParticleInformation::Flags flags = event->particle()->information()->flags();
  ParticleInformation::Flags required = ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet;
  if ((flags & required) != required)
    return;
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
  if (nTofHits > 0)
    histogram()->Fill(track->rigidity(), totSum / nTofHits);
}

void TOTMomentumCorrelation::finalize() {
  setDrawOption(CONT4Z);
}

