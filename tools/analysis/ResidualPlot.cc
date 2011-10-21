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

ResidualPlot::ResidualPlot(Enums::AnalysisTopic topic, Layer* layer)
  : AnalysisPlot(topic)
  , H2DProjectionPlot()
  , m_layer(layer)
{
  controlWidget()->spinBox()->setMaximum(1024);
  controlWidget()->spinBox()->setSingleStep(10);
  controlWidget()->spinBox()->setValue(32);
  setTitle(QString("Residuals layer at %1").arg(layer->z()));

  double max = 0.;
  if (topic == Enums::ResidualsTracker || topic == Enums::MonteCarloTracker)
    max = 3.;
  if (topic == Enums::ResidualsTRD || topic == Enums::MonteCarloTRD)
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

void ResidualPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();

  if (!track)
    return;

  if (!event->flagsSet(ParticleInformation::AllTrackerLayers))
    return;

  // remove hits in this layer from hits for track fit
  QVector<Hit*> hitsInThisLayer;

  const QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    Hit* hit = *it;
    double z = round(hit->position().z()*100)/100.;
    if (fabs(z - m_layer->z()) <= 5)
      hitsInThisLayer.push_back(hit);
  }

  Track* mytrack = referenceTrack(event);

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

      unsigned short detId = hit->elementId();
      unsigned short index = m_layer->detIds().indexOf(detId);
      DetectorElement* element = Setup::instance()->element(detId);
      unsigned short nChannels = element->nChannels();
      unsigned short channel = element->sortedChannel(hit->channel());

      histogram()->Fill(index*nChannels + channel, res);
    }
    delete mytrack;
  }
}

Track* ResidualPlot::referenceTrack(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();

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
  if (track->type() == Enums::StraightLine)
    mytrack = new StraightLine;
  else if (track->type() == Enums::BrokenLine)
    mytrack = new BrokenLine;
  else if (track->type() == Enums::CenteredBrokenLine)
    mytrack = new CenteredBrokenLine;
  else if (track->type() == Enums::CenteredBrokenLine2D)
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
