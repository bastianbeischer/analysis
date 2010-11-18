#include "ResidualPlot.hh"

#include "TrackSelection.hh"
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
  : AnalysisPlot(topic)
  , H2DPlot()
  , m_layer(layer)
{
  setTitle(QString("Residuals layer at %1").arg(layer->z()));

  double max = 0.;
  if (topic == AnalysisPlot::ResidualsUpperTracker || AnalysisPlot::ResidualsLowerTracker)
    max = 3.;
  if (topic == AnalysisPlot::ResidualsTRD)
    max = 10.;

  TH2D* histogram = new TH2D(qPrintable(title()+QString::number(id())), "", layer->nElements(), 0, layer->nElements(), 200, -max, max);
  histogram->GetXaxis()->SetTitle("SiPM array number");
  histogram->GetYaxis()->SetTitle("residue / mm");
  setHistogram(histogram);
}

ResidualPlot::~ResidualPlot()
{
}

void ResidualPlot::processEvent(const QVector<Hit*>& hits, Track* track, TrackSelection* selection, SimpleEvent* /*event*/)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !selection || !track->fitGood())
    return;

  TrackSelection::Flags flags = selection->flags();
  if (!(flags & TrackSelection::AllTrackerLayers))
    return;

  // only select tracks which didn't pass through the magnet
  if ((flags & TrackSelection::MagnetCollision))
    return;

  // remove hits in this layer from hits for track fit
  QVector<Hit*> hitsForFit;
  QVector<Hit*> hitsInThisLayer;
      

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
