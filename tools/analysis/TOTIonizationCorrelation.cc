#include "TOTIonizationCorrelation.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Constants.hh"
#include "ParticleInformation.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>

TOTIonizationCorrelation::TOTIonizationCorrelation(Hit::ModuleType type) 
: H2DPlot() 
, TOTLayer(TOTLayer::All)
, m_type(type)
{
  QString typeName;
  if (m_type == Hit::trd) {
    typeName = "trd";
  }
  if (m_type == Hit::tracker) {
    typeName = "tracker";
  }
  m_plotName = QString("time over threshold %1 signal height correlation").arg(typeName);
}

TOTIonizationCorrelation::TOTIonizationCorrelation(TOTLayer::Layer layer, Hit::ModuleType type)
  : H2DPlot()
  , TOTLayer(layer)
  , m_type(type)
{
  unsigned int nBinsX = 150;
  double xMin = 0;
  double xMax = 1e5;
  QString typeName;
  if (m_type == Hit::trd) {
    typeName = "trd";
    nBinsX = 150;
    xMin = 0;
    xMax = 7e2;
  }
  if (m_type == Hit::tracker) {
    typeName = "tracker";
    nBinsX = 150;
    xMin = 0;
    xMax = 7e4;
  }
  QString title = m_plotName + " " + layerName(layer);
  setTitle(title);
  TH2D* histogram = new TH2D(qPrintable(title), "", nBinsX, xMin, xMax, 100, 0, 100);
  setAxisTitle("signal height / ADC counts", "mean time over threshold / ns", "");
  addHistogram(histogram);
}

TOTIonizationCorrelation::~TOTIonizationCorrelation()
{}

TOTIonizationCorrelation* TOTIonizationCorrelation::create(TOTLayer::Layer layer) const
{ 
  return new TOTIonizationCorrelation(layer, m_type); 
}

void TOTIonizationCorrelation::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  const QVector<Hit*>& clusters = track->hits();
  ParticleInformation::Flags flags = particle->information()->flags();
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
    double nonTofSignal = sumOfNonTOFSignalHeights(track, clusters);
    histogram()->Fill(nonTofSignal, totSum / nTofHits);
  }
}

double TOTIonizationCorrelation::sumOfNonTOFSignalHeights(const Track*, const QVector<Hit*>& clusters) {
  double sumSignal = 0;
  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() != Hit::tof && hit->type() == m_type) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      const std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        sumSignal+= (*it)->signalHeight();
      }
    }
  }
  return sumSignal;
}

void TOTIonizationCorrelation::finalize()
{
  setDrawOption(CONT4Z);
}
