#include "TOTDetectorIonisationCorrelation.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Track.hh"
#include "Constants.hh"
#include "TrackInformation.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>

TOTDetectorIonisationCorrelation::TOTDetectorIonisationCorrelation(QString layer, QString typ)
  : AnalysisPlot(TimeOverThreshold)
  , H2DPlot()
  , m_layer(layer)
{
  const unsigned int nBinsY = 150;
  const double yMin = 0;
  const double yMax = 300;
  unsigned int nBinsX = 150;
  double xMin = 0;
  double xMax = 1e5;
  if (typ == "trd") {
    m_typ = Hit::trd;
    nBinsX = 150;
    xMin = 0;
    xMax = 7e2;
  }
  if (typ == "tracker") {
    m_typ = Hit::tracker;
    nBinsX = 150;
    xMin = 0;
    xMax = 7e4;
  }
  QString title = QString("sum time over threshold "+typ+" signal hight correlation "+layer+" layer");
  setTitle(title);
  TH2D* histogram = new TH2D(qPrintable(title), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetYaxis()->SetTitle("time over threshold / ns");
  histogram->GetXaxis()->SetTitle("total signal height / ADC-Counts");
  setHistogram(histogram);
}

TOTDetectorIonisationCorrelation::~TOTDetectorIonisationCorrelation()
{}

void TOTDetectorIonisationCorrelation::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*) {
  
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
      double nonTofSignal = sumOfNonTOFSignalHeights(track, hits);
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
      histogram()->Fill(nonTofSignal, totSum);
    }
  }
}

double TOTDetectorIonisationCorrelation::sumOfNonTOFSignalHeights(Track* /*track*/, QVector<Hit*> clusters) {
	double sumSignal = 0;
	foreach(Hit* clusterHit, clusters) {
		if (clusterHit->type() != Hit::tof && clusterHit->type() == m_typ) {
      Cluster* cluster = static_cast<Cluster*>(clusterHit);
      foreach(Hit* hit, cluster->hits()){
        sumSignal += hit->signalHeight();
      }
		}
	}
	return sumSignal;
}

bool TOTDetectorIonisationCorrelation::checkLayer(double z)
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
