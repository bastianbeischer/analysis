#include "TOTTimeCorrelationPlot.hh"

#include "Setup.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Constants.hh"
#include "Corrections.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <math.h>

TOTTimeCorrelationPlot::TOTTimeCorrelationPlot(unsigned int id, QDateTime first, QDateTime last)
  : AnalysisPlot(Enums::TimeOverThreshold)
  , H2DProjectionPlot()
  , m_id(id)
{
  QString title = QString("time over threshold time correlation 0x%1").arg(0x8000 | id, 0, 16);
  setTitle(title);
  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  const unsigned int nTimeBins = (t2 - t1) / 60;
  const unsigned int nTotBins = 100;
  const double minTot = 0;
  const double maxTot = 75;
  TH2D* histogram = new TH2D(qPrintable(title), "", nTimeBins, t1, t2, nTotBins, minTot, maxTot);
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
  setAxisTitle("time", "time over threshold / ns", "");
  addHistogram(histogram);
}

TOTTimeCorrelationPlot::~TOTTimeCorrelationPlot() 
{
}

void TOTTimeCorrelationPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;

  const QVector<Hit*>& hits = track->hits();
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      const std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        if (tofHit->detId() == m_id) {
          unsigned int eventTime = event->simpleEvent()->time();
          TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
          histogram()->Fill(eventTime, tofSipmHit->timeOverThreshold());
        }
      }
    }
  }
}
