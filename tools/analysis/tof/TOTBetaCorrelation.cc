#include "TOTBetaCorrelation.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"

#include <TH2D.h>
#include <TLatex.h>

#include <QDebug>

TOTBetaCorrelation::TOTBetaCorrelation() : H2DPlot() , TOTLayer(TOTLayer::All)
{
  m_plotName = "time over threshold beta correlation";
}

TOTBetaCorrelation::TOTBetaCorrelation(TOTLayer::Layer layer)
  : H2DPlot()
  , TOTLayer(layer)
{
  QString htitle = m_plotName + " " + layerName(layer) + " tof";
  setTitle(htitle);
  TH2D* histogram = new TH2D(qPrintable(htitle), "", 100, 0, 1.6, 150, 0, 100);
  setAxisTitle("beta", "sum time over threshold / ns", "");
  addHistogram(histogram);
}

TOTBetaCorrelation::~TOTBetaCorrelation()
{}

TOTBetaCorrelation* TOTBetaCorrelation::create(TOTLayer::Layer layer) const
{ 
  return new TOTBetaCorrelation(layer); 
}

void TOTBetaCorrelation::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  const QVector<Hit*>& clusters = track->hits();
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
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
      std::vector<Hit*>& subHits = tofCluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(*it);
        totSum+= tofSipmHit->timeOverThreshold();
        ++nTofHits;
      }
    }
  }
  if (nTofHits > 0)
    histogram()->Fill(particle->beta(), totSum / nTofHits);
}
  
void TOTBetaCorrelation::finalize()
{
  setDrawOption(CONT4Z);
}
