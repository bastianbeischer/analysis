#include "ResidualPlot.hh"

#include "ParticleInformation.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "Setup.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "StraightLine.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"
#include "CenteredBrokenLine2D.hh"
#include "Particle.hh"
#include "Track.hh"
#include "TH2D.h"
#include "ProjectionControlWidget.hh"

#include <QSpinBox>

#include <TVector3.h>

#include <cmath>
#include <iostream>

ResidualPlot::ResidualPlot(AnalysisPlot::Topic topic, Layer* layer)
  : AnalysisPlot(topic)
  , H2DProjectionPlot()
  , m_layer(layer)
{
  controlWidget()->spinBox()->setMaximum(1024);
  controlWidget()->spinBox()->setSingleStep(10);
  controlWidget()->spinBox()->setValue(32);
  setTitle(QString("Residuals layer at %1").arg(layer->z()));

  double max = 0.;
  if (topic == AnalysisPlot::ResidualsTracker || topic == AnalysisPlot::MonteCarloTracker)
    max = 3.;
  if (topic == AnalysisPlot::ResidualsTRD || topic == AnalysisPlot::MonteCarloTRD)
    max = 10.;

  unsigned short nElements = layer->nElements();
  unsigned short nChannels = layer->elements().first()->nChannels();
  TH2D* histogram = new TH2D(qPrintable(title()), "", nElements*nChannels, 0, nElements*nChannels, 200, -max, max);

  setAxisTitle("channel number", "residue / mm", "");
  addHistogram(histogram);
}

ResidualPlot::~ResidualPlot()
{
}

void ResidualPlot::processEvent(const QVector<Hit*>& hits, const Particle* const particle, const SimpleEvent* const event)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  // if (!(flags & ParticleInformation::Chi2Good))
  //   return;

  // only select tracks which didn't pass through the magnet
  //if ((flags & ParticleInformation::MagnetCollision))
  //  return;

  // remove hits in this layer from hits for track fit
  QVector<Hit*> hitsInThisLayer;

  const QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    Hit* hit = *it;
    double z = round(hit->position().z()*100)/100.;
    if (fabs(z - m_layer->z()) <= 5)
      hitsInThisLayer.push_back(hit);
  }

  Track* mytrack = referenceTrack(hits, particle, event);

  // fit and fill histograms
  if (mytrack) {
    QVector<Hit*>::const_iterator layerEndIt = hitsInThisLayer.end();
    for (QVector<Hit*>::const_iterator it = hitsInThisLayer.begin(); it != layerEndIt; ++it) {
      Hit* hit = *it;
      double z = m_layer->z();
      double hitX = hit->position().x();
      double hitY = hit->position().y();
      double trackX = mytrack->x(z);
      double trackY = mytrack->y(z);

      double angle = (-1.)*hit->angle();
      double c = cos(angle);
      double s = sin(angle);
      double hitU = c*hitX - s*hitY;
      double trackU = c*trackX - s*trackY;

      double res = hitU - trackU;

      unsigned short detId = hit->detId() - hit->channel();
      unsigned short index = m_layer->detIds().indexOf(detId);
      DetectorElement* element = Setup::instance()->element(detId);
      unsigned short nChannels = element->nChannels();
      unsigned short channel = hit->channel();
      if (strcmp(hit->ClassName(), "Cluster") == 0) {
        int max = 0;
        int imax = 0;
        Cluster* cluster = static_cast<Cluster*>(hit);
        std::vector<Hit*> subHits = cluster->hits();
        for (unsigned int i = 0 ; i < subHits.size(); i++) {
          if (subHits.at(i)->signalHeight() > max) {
            max = subHits.at(i)->signalHeight();
            imax = i;
          }
        }
        channel = element->sortedChannel(subHits.at(imax)->channel());
      }

      histogram()->Fill(index*nChannels + channel, res);
    }
    delete mytrack;
  }
}

Track* ResidualPlot::referenceTrack(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const /*event*/)
{
  const Track* track = particle->track();

  // remove hits in this layer from hits for track fit
  QVector<Hit*> hitsForFit;

  const QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    Hit* hit = *it;
    double z = round(hit->position().z()*100)/100.;
    if (fabs(z - m_layer->z()) > 5)
      hitsForFit.push_back(hit);
  }

  Track* mytrack = 0;
  if (track->type() == Track::StraightLine)
    mytrack = new StraightLine;
  else if (track->type() == Track::BrokenLine)
    mytrack = new BrokenLine;
  else if (track->type() == Track::CenteredBrokenLine)
    mytrack = new CenteredBrokenLine;
  else if (track->type() == Track::CenteredBrokenLine2D)
    mytrack = new CenteredBrokenLine2D;
  else mytrack = 0;

  if (!mytrack)
    return 0;
  else if(mytrack->fit(hitsForFit))
    return mytrack;
  else {
    delete mytrack;
    return 0;
  }
}
