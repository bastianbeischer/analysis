#include "TOTLayerPlot.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Constants.hh"
#include "ParticleInformation.hh"

#include <TH1D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>

TOTLayerPlot::TOTLayerPlot() : H1DPlot() , TOTLayer(TOTLayer::All)
{
  m_plotName = "time over threshold per layer";
}

TOTLayerPlot::TOTLayerPlot(TOTLayer::Layer layer)
  : H1DPlot()
  , TOTLayer(layer)
{
  QString title = QString("time over threshold "+layerName(layer)+" layer");
  setTitle(title);
  TH1D* histogram = new TH1D(qPrintable(title), "", 150, 0, 100);
  setAxisTitle("mean time over threshold / ns", "");
  addHistogram(histogram);
}

TOTLayerPlot::~TOTLayerPlot()
{}

TOTLayerPlot* TOTLayerPlot::create(TOTLayer::Layer layer) const
{ 
  return new TOTLayerPlot(layer); 
}

void TOTLayerPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  if (!track || !track->fitGood())
    return;
  const QVector<Hit*>& clusters = track->hits();
  ParticleInformation::Flags flags = event->particle()->information()->flags();
  ParticleInformation::Flags required = ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet;
  if ((flags & required) != required)
    return;
  double totSum = 0.;
  int nTofHits = 0;
  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() == Hit::tof) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
      double z = tofCluster->position().z();
      if (qAbs(track->x(z) - tofCluster->position().x()) > 0.95 * Constants::tofBarWidth / 2.)
        continue;
      if (!checkLayer(z))
        continue;
      const std::vector<Hit*>& subHits = tofCluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(*it);
        totSum+= tofSipmHit->timeOverThreshold();
        ++nTofHits;
      }
    }
  }
  if (nTofHits > 0) {
    histogram()->Fill(totSum / nTofHits);
  }
} 

