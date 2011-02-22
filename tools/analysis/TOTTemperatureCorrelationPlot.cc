#include "TOTTemperatureCorrelationPlot.hh"

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

TOTTemperatureCorrelationPlot::TOTTemperatureCorrelationPlot(unsigned int id)
  : AnalysisPlot(TimeOverThreshold)
  , H2DPlot()
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
  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetXaxis()->SetTitle("#vartheta /  #circC");
  histogram->GetYaxis()->SetTitle("time over threshold / ns");
  setHistogram(histogram);
}

TOTTemperatureCorrelationPlot::~TOTTemperatureCorrelationPlot() 
{
}

void TOTTemperatureCorrelationPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* event) {
  foreach(Hit* hit, hits) {
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > 1.2 * Constants::tofBarWidth / 2)
        continue;
      foreach(Hit* tofHit, cluster->hits()) {
        if (tofHit->detId() == m_id) {
          double temperature = event->sensorData(Setup::instance()->idToSensor(m_id));
          TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
          histogram()->Fill(temperature, tofSipmHit->timeOverThreshold());
        }
      }
    }
  }
}
