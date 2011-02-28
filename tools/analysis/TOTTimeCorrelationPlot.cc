#include "TOTTimeCorrelationPlot.hh"

#include "Setup.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
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

TOTTimeCorrelationPlot::TOTTimeCorrelationPlot(unsigned int id)
  : AnalysisPlot(TimeOverThreshold)
  , H2DPlot()
  , m_id(id)
  , m_minTime(1964373878) //flight 1964373878 //float 2016232280
  , m_maxTime(2023068183) //flight 2023068183 //float 2021732904
{
  QString title = QString("time over threshold time correlation 0x%1").arg(0x8000 | id, 0, 16);
  setTitle(title);
  const unsigned int nTimeBins = 100;

  const unsigned int nTotBins = 100;
  const double minTot = 0;
  const double maxTot = 75;
  TH2D* histogram = new TH2D(qPrintable(title), "", nTimeBins, convertTime(m_minTime), convertTime(m_maxTime), nTotBins, minTot, maxTot);
  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetXaxis()->SetTitle("time / min");
  histogram->GetYaxis()->SetTitleOffset(1.4);
  histogram->GetYaxis()->SetTitle("time over threshold / ns");
  setHistogram(histogram);
}

TOTTimeCorrelationPlot::~TOTTimeCorrelationPlot() 
{
}

void TOTTimeCorrelationPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* event)
{
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > 1.2 * Constants::tofBarWidth / 2)
        continue;
      std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        if (tofHit->detId() == m_id) {
          unsigned int eventTime = event->runStartTime() *1000 + event->eventTime();
          TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
          histogram()->Fill(convertTime(eventTime), tofSipmHit->timeOverThreshold());
        }
      }
    }
  }
}

inline double TOTTimeCorrelationPlot::msToMin(double timeInMs)
{
  return timeInMs / 1000. / 60.;
}

inline double TOTTimeCorrelationPlot::convertTime(unsigned int time)
{
  return msToMin(time - m_minTime);
}

