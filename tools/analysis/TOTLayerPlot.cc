#include "TOTLayerPlot.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Track.hh"
#include "Constants.hh"
#include "TrackInformation.hh"

#include <TH1D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>

TOTLayerPlot::TOTLayerPlot(TofLayer layer)
  : AnalysisPlot(TimeOverThreshold)
  , H1DPlot()
  , m_layer(layer)
{
  QString title = QString("time over threshold "+layerName(layer)+" layer");
  setTitle(title);
  TH1D* histogram = new TH1D(qPrintable(title), "", 150, 0, 100);
  setAxisTitle("mean time over threshold / ns", "");
  addHistogram(histogram);
}

TOTLayerPlot::~TOTLayerPlot()
{}

void TOTLayerPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
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
  if (nTofHits > 0) {
    histogram()->Fill(totSum / nTofHits);
  }
} 

QString TOTLayerPlot::layerName(TofLayer layer)
{
  switch (layer) {
    case Lower: return "lower";
    case Upper: return "upper";
    case All: return "total";
  }
  return QString();
}

bool TOTLayerPlot::checkLayer(double z)
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
