#include "TOTPerBarPlot.hh"

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

TOTPerBarPlot::TOTPerBarPlot()
  : AnalysisPlot(TimeOverThreshold)
  , H2DPlot()
{
  QString title = QString("time over threshold per bar");
  setTitle(title);
  const unsigned int nBinsX = 64;
  const double xMin = -0.5;
  const double xMax = 15.5;
  const unsigned int nBinsY = 150;
  const double yMin = 0;
  const double yMax = 100;
  TH2D* histogram = new TH2D(qPrintable(title), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  for (int ch = 0; ch < 64; ch += 4)
    histogram->GetXaxis()->SetBinLabel(1 + ch, qPrintable(QString("%1").arg(0x8000 | ch, 0, 16)));
  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetXaxis()->SetTitle("bar");
  histogram->GetYaxis()->SetTitle("mean time over threshold / ns");
  histogram->GetYaxis()->SetTitleOffset(1.4);
  setHistogram(histogram);
}

TOTPerBarPlot::~TOTPerBarPlot()
{}

void TOTPerBarPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*) {
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > Constants::tofBarWidth)
        continue;
      std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      double totSum[TOTPerBarPlot::nTofBars];
      int nTofHits[TOTPerBarPlot::nTofBars];
      for (unsigned int bar = 0; bar < TOTPerBarPlot::nTofBars; bar++) {
        totSum[bar] = 0;
        nTofHits[bar] = 0;
      }
      int tofBar = -1;
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        if (tofHit->detId() == 0x8034) {
          continue;
        }
        tofBar = (int)((tofHit->detId() - 0x8000) / 4);
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
        totSum[tofBar] += tofSipmHit->timeOverThreshold();
        nTofHits[tofBar]++;
      }
      histogram()->Fill(tofBar, totSum[tofBar]/nTofHits[tofBar]);
    }
  }
}
