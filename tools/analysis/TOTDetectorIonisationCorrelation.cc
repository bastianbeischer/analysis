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

TOTDetectorIonisationCorrelation::TOTDetectorIonisationCorrelation(TofLayer layer, Hit::ModuleType type)
  : AnalysisPlot(TimeOverThreshold)
  , H2DPlot()
  , m_layer(layer)
  , m_type(type)
{
  const unsigned int nBinsY = 150;
  const double yMin = 0;
  const double yMax = 100;
  unsigned int nBinsX = 150;
  double xMin = 0;
  double xMax = 1e5;
  QString typeName;
  if (m_type == Hit::trd) {
    typeName = "trd";
    nBinsX = 150;
    xMin = 0;
    xMax = 7e2;
  }
  if (m_type == Hit::tracker) {
    typeName = "tracker";
    nBinsX = 150;
    xMin = 0;
    xMax = 7e4;
  }
  QString title = QString("time over threshold "+typeName+" signal hight correlation "+layer+" layer");
  setTitle(title);
  TH2D* histogram = new TH2D(qPrintable(title), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetYaxis()->SetTitle("mean time over threshold / ns");
  histogram->GetYaxis()->SetTitleOffset(1.4);
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
      int nTofHits = 0;
      double nonTofSignal = sumOfNonTOFSignalHeights(track, hits);
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
      histogram()->Fill(nonTofSignal, totSum/nTofHits);
    }
  }
}

void TOTDetectorIonisationCorrelation::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  histogram()->Draw("cont4z");
}

double TOTDetectorIonisationCorrelation::sumOfNonTOFSignalHeights(Track* /*track*/, QVector<Hit*> clusters) {
	double sumSignal = 0;
	foreach(Hit* clusterHit, clusters) {
		if (clusterHit->type() != Hit::tof && clusterHit->type() == m_type) {
      Cluster* cluster = static_cast<Cluster*>(clusterHit);
      foreach(Hit* hit, cluster->hits()){
        sumSignal += hit->signalHeight();
      }
		}
	}
	return sumSignal;
}

QString TOTDetectorIonisationCorrelation::layerName(TofLayer layer)
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

bool TOTDetectorIonisationCorrelation::checkLayer(double z)
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
