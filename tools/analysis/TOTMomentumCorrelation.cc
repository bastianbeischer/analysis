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

TOTMomentumCorrelation::TOTMomentumCorrelation(unsigned int id)
  : AnalysisPlot(AnalysisPlot::TimeOverThreshold)
  , H2DPlot()
  , m_id(id)
{
  
  QString htitle =QString("time over threshold momentum correlation 0x%1").arg(m_id, 0, 16);
  setTitle(htitle);
  TH2D* histogram = new TH2D(qPrintable(title()), "", 50, 0, 5, 75, 0, 75);
  histogram->GetXaxis()->SetTitle("rigidity / GV");
  histogram->GetYaxis()->SetTitle("time over threshold / ns");
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
      std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        if (tofHit->detId() == m_id) {
          TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*> (tofHit);
          histogram()->Fill(track->pt(), tofSipmHit->timeOverThreshold());
        }
      }
    }
  }
}

void TOTMomentumCorrelation::update() {
  latex()->SetTitle(qPrintable(QString("#rho = %1").arg(histogram()->GetCorrelationFactor())));
}
