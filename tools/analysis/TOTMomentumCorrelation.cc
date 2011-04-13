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

TOTMomentumCorrelation::TOTMomentumCorrelation(TofLayer layer)
: AnalysisPlot(AnalysisPlot::TimeOverThreshold)
, H2DPlot()
, m_layer(layer)
{
  QString htitle = "time over threshold momentum correlation " + layerName(layer) + " tof";
  setTitle(htitle);
  TH2D* histogram = new TH2D(qPrintable(htitle), "", 100, 0, 10, 150, 0, 100);
  setAxisTitle("rigidity / GV", "mean time over threshold / ns", "");
  addHistogram(histogram);
}

TOTMomentumCorrelation::~TOTMomentumCorrelation()
{}

void TOTMomentumCorrelation::processEvent(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
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
      std::vector<Hit*>& subHits = tofCluster->hits();
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

QString TOTMomentumCorrelation::layerName(TofLayer layer)
{
  switch (layer) {
    case Lower: return "lower";
    case Upper: return "upper";
    case All: return "all";
  }
  return QString();
}

bool TOTMomentumCorrelation::checkLayer(double z)
{
  if (m_layer == Upper && z > 0) {
    return true;
  } else if (m_layer == Lower && z < 0) {
    return true;
  } else if (m_layer == All) {
    return true;
  }
  return false;
}
