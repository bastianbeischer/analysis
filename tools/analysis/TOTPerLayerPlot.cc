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

TOTPerLayerPlot::TOTPerLayerPlot(QString layer)
  : AnalysisPlot(TimeOverThreshold)
  , H1DPlot()
  , m_layer(layer)
{
  QString title = QString("time over threshold "+layer+" layer");
  setTitle(title);
  const unsigned int nBinsX = 150;
  const double xMin = 0;
  const double xMax = 300;
  TH1D* histogram = new TH1D(qPrintable(title), "", nBinsX, xMin, xMax);
  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetXaxis()->SetTitle("sum time over threshold / ns");
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
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        if (tofHit->detId() == 0x8034) {
          continue;
        }
        int tofBar = (int)((tofHit->detId() - 0x8000) / 4);
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
        double tot = tofSipmHit->timeOverThreshold();
        if (tofBar == 13) {// bar with one sipm damaged
          tot *= 4/3.;
        }
        totSum += tot;
      }
      histogram()->Fill(totSum);
    }
  }
}

bool TOTPerLayerPlot::checkLayer(double z)
{
  if (m_layer == "upper") {
    if (z > 0) {
      return true;
    }
    else {
      return false;
    }      
  }
  else if (m_layer == "lower") {
    if (z < 0) {
      return true;
    }
    else {
      return false;
    }      
  }
  else if (m_layer == "total") {
    return true;
  }
  else {
    qFatal("Wrong argument passed to TOTPerLayer!");
    return false;
  }
}
