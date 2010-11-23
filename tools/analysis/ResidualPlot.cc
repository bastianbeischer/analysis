#include "ResidualPlot.hh"

#include "TrackSelection.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "Setup.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "StraightLine.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"
#include "Track.hh"
#include "TH2D.h"

#include <TVector3.h>

#include <cmath>
#include <iostream>

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

  unsigned short nElements = layer->nElements();
  unsigned short nChannels = layer->elements().first()->nChannels();
  TH2D* histogram = new TH2D(qPrintable(title()+QString::number(id())), "", nElements*nChannels, 0, nElements*nChannels, 200, -max, max);

  if (topic == AnalysisPlot::ResidualsUpperTracker || AnalysisPlot::ResidualsLowerTracker) {
    histogram->GetXaxis()->SetTitle("SiPM array number");
  }
  if (topic == AnalysisPlot::ResidualsTRD) {
    histogram->GetXaxis()->SetTitle("TRD module number");
  }
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
    double z = round(hit->position().z()*100)/100.;
    if (fabs(z - m_layer->z()) > 5) {
      hitsForFit.push_back(hit);
    }
    else {
      hitsInThisLayer.push_back(hit);
    }
  }

  Track* mytrack = 0;
  if (track->type() == Track::StraightLine)
    mytrack = new StraightLine;
  else if (track->type() == Track::BrokenLine)
    mytrack = new BrokenLine;
  else if (track->type() == Track::CenteredBrokenLine)
    mytrack = new CenteredBrokenLine;
  else mytrack = 0;

  // fit and fill histograms
  if (mytrack->fit(hitsForFit)) {
    foreach(Hit* hit, hitsInThisLayer) {
      //      TVector3 pos = 0.5* (hit->position() + hit->counterPosition());
      TVector3 pos = Setup::instance()->positionForHit(hit);
      TVector3 trackPos = mytrack->position(m_layer->z());

      double angle = hit->angle();
      pos.RotateZ(-angle);
      trackPos.RotateZ(-angle);

      double res = (pos - trackPos).x();
      unsigned short detId = hit->detId() - hit->channel();
      unsigned short index = m_layer->detIds().indexOf(detId);
      unsigned short nChannels = Setup::instance()->element(detId)->nChannels();
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
        channel = subHits.at(imax)->channel();
      }

      double alignmentShift = Setup::instance()->element(detId)->alignmentShift();

      histogram()->Fill(index*nChannels + channel, res-alignmentShift);
    }
  }
  delete mytrack;
}

void ResidualPlot::finalize()
{
}
