#include "TOTMomentumCorrelation.hh"

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

TOTMomentumCorrelation::TOTMomentumCorrelation(TofLayer layer)
: AnalysisPlot(AnalysisPlot::TimeOverThreshold)
, H2DPlot()
, m_layer(layer)
{
  
  QString htitle = "time over threshold momentum correlation "+layerName(layer)+" tof";
  setTitle(htitle);
  const unsigned int nBinsX = 100;
  const double xMin = 0;
  const double xMax = 10;
  const unsigned int nBinsY = 150;
  const double yMin = 0;
  const double yMax = 70;
  TH2D* histogram = new TH2D(qPrintable(htitle), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  
  histogram->GetXaxis()->SetTitle("rigidity / GV");
  histogram->GetYaxis()->SetTitle("mean time over threshold / ns");
  //histogram->GetYaxis()->SetTitleOffset(4);
  setHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
}

TOTMomentumCorrelation::~TOTMomentumCorrelation()
{}


void TOTMomentumCorrelation::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & (TrackInformation::AllTrackerLayers | TrackInformation::InsideMagnet)))
    return;

  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator it = clusters.begin(); it != endIt; ++it) {
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
        int tofBar = (int)((tofHit->detId() - 0x8000) / 4);
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
        double tot = tofSipmHit->timeOverThreshold();
        if (tofBar == 13) {// bar with one sipm damaged
          tot *= 4/3.;
        }
        totSum += tot;
        nTofHits++;
      }
      histogram()->Fill(track->pt(), totSum/nTofHits);
    }
  }
}

void TOTMomentumCorrelation::update() {
  latex()->SetTitle(qPrintable(QString("#rho = %1").arg(histogram()->GetCorrelationFactor())));
}

void TOTMomentumCorrelation::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  histogram()->Draw("cont4z");
}

QString TOTMomentumCorrelation::layerName(TofLayer layer)
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

bool TOTMomentumCorrelation::checkLayer(double z)
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
