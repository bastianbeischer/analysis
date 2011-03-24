#include "TOTPerLayerPlot.hh"

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

TOTPerLayerPlot::TOTPerLayerPlot(TofLayer layer)
  : AnalysisPlot(TimeOverThreshold)
  , H1DPlot()
  , m_layer(layer)
{
  QString title = QString("time over threshold "+layerName(layer)+" layer");
  setTitle(title);
  const unsigned int nBinsX = 150;
  const double xMin = 0;
  const double xMax = 100;
  TH1D* histogram = new TH1D(qPrintable(title), "", nBinsX, xMin, xMax);
  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetXaxis()->SetTitle("mean time over threshold / ns");
  histogram->GetYaxis()->SetTitleOffset(1.4);
  addHistogram(histogram);
}

TOTPerLayerPlot::~TOTPerLayerPlot()
{}

void TOTPerLayerPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*) {
  
//  if (!track || !track->fitGood())
//    return;
//  TrackInformation::Flags flags = track->information()->flags();
//  if (!(flags & (TrackInformation::AllTrackerLayers | TrackInformation::InsideMagnet)))
//    return;
  
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > Constants::tofBarWidth)
        continue;
      if ( !checkLayer( cluster->position().z() ))
        continue;
      std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      double totSum = 0;
      int nTofHits = 0;
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        if (tofHit->detId() == 0x8034) {
          continue;
        }
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
        double tot = tofSipmHit->timeOverThreshold();
        totSum += tot;
        nTofHits++;
      }
      histogram()->Fill(totSum/nTofHits);
    }
  }
}

QString TOTPerLayerPlot::layerName(TofLayer layer)
{
  switch (layer) {
    case LOWER:
      return "lower";
      break;
    case UPPER:
      return "upper";
      break;
    case TOTAL:
      return "total";
      break;
    default:
      return "not found";
      break;
  }
}

bool TOTPerLayerPlot::checkLayer(double z)
{
  if (m_layer == UPPER && z > 0) {
    return true;
  }
  else if (m_layer == LOWER && z < 0) {
    return true;
  }
  else if (m_layer == TOTAL) {
    return true;
  }
  else {
    return false;
  }
}