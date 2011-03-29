#include "TOTPlot.hh"

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

TOTPlot::TOTPlot()
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

TOTPlot::~TOTPlot()
{}

void TOTPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*) {
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > .95 * Constants::tofBarWidth / 2.)
        continue;
      std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
        histogram()->Fill(tofHit->detId() - 0x8000, tofSipmHit->timeOverThreshold());
      }
    }
  }
}
