#include "TOTDetectorIonisationCorrelation.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Track.hh"
#include "Constants.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>

TOTDetectorIonisationCorrelation::TOTDetectorIonisationCorrelation(QString layer)
  : AnalysisPlot(TimeOverThreshold)
  , H2DPlot()
  , m_layer(layer)
{
  QString title = QString("time over threshold signal hight correlation "+layer+" layer");
  setTitle(title);
  const unsigned int nBinsX = 150;
  const double xMin = 0;
  const double xMax = 1e5;
  const unsigned int nBinsY = 150;
  const double yMin = 0;
  const double yMax = 300;
  TH2D* histogram = new TH2D(qPrintable(title), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetYaxis()->SetTitle("time over threshold / ns");
  histogram->GetXaxis()->SetTitle("total non TOF signal height / ADC-Counts");
  setHistogram(histogram);
}

TOTDetectorIonisationCorrelation::~TOTDetectorIonisationCorrelation()
{}

void TOTDetectorIonisationCorrelation::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*) {
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
		if (clusterHit->type() != Hit::tof) {
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
