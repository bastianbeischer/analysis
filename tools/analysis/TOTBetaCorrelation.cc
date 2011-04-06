#include "TOTBetaCorrelation.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "Constants.hh"

#include <TH2D.h>
#include <TLatex.h>

#include <QDebug>

TOTBetaCorrelation::TOTBetaCorrelation(TofLayer layer)
  : AnalysisPlot(AnalysisPlot::TimeOverThreshold)
  , H2DPlot()
  , m_layer(layer)
{
  QString htitle = "time over threshold beta correlation " + layerName(layer) + " tof";
  setTitle(htitle);
  TH2D* histogram = new TH2D(qPrintable(htitle), "", 100, 0, 1.6, 150, 0, 100);
  setAxisTitle("beta", "sum time over threshold / ns", "");
  addHistogram(histogram);
}

TOTBetaCorrelation::~TOTBetaCorrelation()
{}


void TOTBetaCorrelation::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & (TrackInformation::Chi2Good | TrackInformation::InsideMagnet)))
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
    histogram()->Fill(track->beta(), totSum / nTofHits);
}

QString TOTBetaCorrelation::layerName(TofLayer layer)
{
  switch (layer) {
    case Lower: return "lower";
    case Upper: return "upper";
    case All: return "all";
  }
  return QString();
}

bool TOTBetaCorrelation::checkLayer(double z)
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
  
void TOTBetaCorrelation::finalize()
{
  setDrawOption(CONT4Z);
}
