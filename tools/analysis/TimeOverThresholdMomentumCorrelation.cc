#include "TimeOverThresholdMomentumCorrelation.hh"

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

TimeOverThresholdMomentumCorrelation::TimeOverThresholdMomentumCorrelation(unsigned int id)
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

TimeOverThresholdMomentumCorrelation::~TimeOverThresholdMomentumCorrelation()
{}


void TimeOverThresholdMomentumCorrelation::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & (TrackInformation::AllTrackerLayers | TrackInformation::InsideMagnet)))
    return;

  foreach(Hit* hit, clusters) {
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > Constants::tofBarWidth)
        continue;
      foreach(Hit* tofHit, cluster->hits()) {
        if (tofHit->detId() == m_id) {
          TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*> (tofHit);
          histogram()->Fill(track->pt(), tofSipmHit->timeOverThreshold());
        }
      }
    }
  }
}

void TimeOverThresholdMomentumCorrelation::update() {
  latex()->SetTitle(qPrintable(QString("#rho = %1").arg(histogram()->GetCorrelationFactor())));
}
