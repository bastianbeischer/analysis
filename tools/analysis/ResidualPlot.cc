#include "ResidualPlot.hh"

#include "Layer.hh"
#include "Hit.hh"
#include "StraightLine.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"
#include "Track.hh"
#include "TH2D.h"

#include <TVector3.h>

#include <cmath>

ResidualPlot::ResidualPlot(AnalysisPlot::Topic topic, Layer* layer) :
  H2DPlot(topic, "res", layer->nElements(), 0, layer->nElements(), 200, -3., 3.),
  m_layer(layer)
{
}

ResidualPlot::~ResidualPlot()
{
}

void ResidualPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* /*event*/)
{
  // remove hits in this layer from hits for track fit
  QVector<Hit*> hitsForFit;
  QVector<Hit*> hitsInThisLayer;
      
  foreach(Hit* hit, hits) {
    // test
    double z = hit->position().z();
    z = round(z*100.)/100.;
    if (z != m_layer->z()) {
      hitsForFit.push_back(hit);
    }
    else {
      hitsInThisLayer.push_back(hit);
    }
  }

  Track* mytrack;
  if (track->type() == Track::StraightLine)
    mytrack = new StraightLine;
  else if (track->type() == Track::BrokenLine)
    mytrack = new BrokenLine;
  else if (track->type() == Track::CenteredBrokenLine)
    mytrack = new CenteredBrokenLine;

  // fit and fill histograms
  if (track->fit(hitsForFit)) {
    foreach(Hit* hit, hitsInThisLayer) {
      
      TVector3 pos = 0.5* (hit->position() + hit->counterPosition());
      TVector3 trackPos = track->position(m_layer->z());

      double angle = hit->angle();
      pos.RotateZ(-angle);
      trackPos.RotateZ(-angle);

      double res = (pos - trackPos).x();
      int index = m_layer->detIds().indexOf(hit->detId() - hit->channel());

      m_histogram->Fill(index, res);
    }
  }
  delete mytrack;
}

void ResidualPlot::finalize()
{
}
