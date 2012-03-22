#include "TOTTemperatureCorrelationPlot.hh"

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
#include <cmath>

TOTTemperatureCorrelationPlot::TOTTemperatureCorrelationPlot(unsigned int id)
  : AnalysisPlot(Enums::TimeOverThreshold)
  , H2DProjectionPlot()
  , m_id(id)
{
  QString title = QString("time over threshold temperature correlation 0x%1").arg(0x8000 | id, 0, 16);
  setTitle(title);
  const unsigned int nTemperatureBins = 100;
  const double minTemperature = -25;
  const double maxTemperature = 35;
  const unsigned int nTotBins = 100;
  const double minTot = 0;
  const double maxTot = 75;
  TH2D* histogram = new TH2D(qPrintable(title), "", nTemperatureBins, minTemperature, maxTemperature, nTotBins, minTot, maxTot);
  setAxisTitle("temperature /  #circC", "time over threshold / ns", "");
  addHistogram(histogram);
  addRequiredEventFlags(Enums::TrackGood);
}

TOTTemperatureCorrelationPlot::~TOTTemperatureCorrelationPlot()
{
}

void TOTTemperatureCorrelationPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();

  const QVector<Hit*>& hits = track->hits();
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > 0.95 * Constants::tofBarWidth / 2.)
        continue;
      const std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        if (tofHit->detId() == m_id) {
          double temperature = event->simpleEvent()->sensorData(Setup::instance()->sensorForId(m_id));
          TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
          if (!std::isnan(temperature))
            histogram()->Fill(temperature, tofSipmHit->timeOverThreshold());
        }
      }
    }
  }
}
