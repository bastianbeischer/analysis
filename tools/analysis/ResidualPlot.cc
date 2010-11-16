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

ResidualPlot::ResidualPlot(AnalysisPlot::Topic topic, Layer* layer)
  : H2DPlot(topic)
  , m_layer(layer)
{
  setTitle(QString("Residuals layer at %1").arg(layer->z()));
  TH2D* histogram = new TH2D(qPrintable(title()+QString::number(id())), "", layer->nElements(), 0, layer->nElements(), 200, -3., 3.);
  histogram->GetXaxis()->SetTitle("SiPM array number");
  histogram->GetYaxis()->SetTitle("residue / mm");
  setHistogram(histogram);
}

ResidualPlot::~ResidualPlot()
{
}

void ResidualPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* /*event*/)
{
  // remove hits in this layer from hits for track fit
  QVector<Hit*> hitsForFit;
  QVector<Hit*> hitsInThisLayer;
      
  // plot only exactly 8 tracker hits
  int nTrackerHits = 0;
  foreach(Hit* hit, hits)
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
    return;

  // only select tracks which didn't pass through the magnet
  double x0 = track->x(0.);
  double y0 = track->y(0.);
  double r = sqrt(x0*x0 + y0*y0);
  if (r > 65 && r < 110)
    return;

  foreach(Hit* hit, hits) {
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

      histogram()->Fill(index, res);
    }
  }
  delete mytrack;
}

void ResidualPlot::finalize()
{
}
