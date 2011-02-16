#include "TimeOverThresholdPlot.hh"

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

TimeOverThresholdPlot::TimeOverThresholdPlot()
	: AnalysisPlot(TimeOverThreshold)
  , H2DPlot()
{
	QString title = QString("time over threshold");
	setTitle(title);
	TH2D* histogram = new TH2D(qPrintable(title), "", 64, -0.5, 63.5, 150, 0, 150);
  for (int ch = 0; ch < 64; ++ch)
    histogram->GetXaxis()->SetBinLabel(1 + ch, qPrintable(QString("%1").arg(0x8000 | ch, 0, 16)));
	histogram->GetXaxis()->SetTitleOffset(1.4);
	histogram->GetXaxis()->SetTitle("channel ID");
	histogram->GetYaxis()->SetTitle("time over threshold / ns");
	setHistogram(histogram);
}

TimeOverThresholdPlot::~TimeOverThresholdPlot()
{}

void TimeOverThresholdPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*) {
  foreach(Hit* hit, hits) {
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > Constants::tofBarWidth)
        continue;
      foreach(Hit* tofHit, cluster->hits()) {
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
        histogram()->Fill(tofHit->detId() - 0x8000, tofSipmHit->timeOverThreshold());
      }
    }
  }
}
