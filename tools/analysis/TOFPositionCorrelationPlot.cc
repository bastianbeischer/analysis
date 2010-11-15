#include "TOFPositionCorrelationPlot.hh"
#include "BrokenLine.hh"

#include "Hit.hh"
#include "TOFCluster.hh"

#include <TH2.h>
#include <TAxis.h>

TOFPositionCorrelationPlot::TOFPositionCorrelationPlot(unsigned short id)
  : H2DPlot(AnalysisPlot::MiscellaneousTOF)
  , m_id(id)
{
  setTitle(QString("occupancy %1").arg(id));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 50, -250, 250, 50, -250, 250);
  histogram->GetXaxis()->SetTitle("y_{tracker} / mm");
  histogram->GetYaxis()->SetTitle("y_{TOF} / mm");
  setHistogram(histogram);
}

TOFPositionCorrelationPlot::~TOFPositionCorrelationPlot()
{}

void TOFPositionCorrelationPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (track) {
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tof && (hit->detId()-hit->channel()) == m_id)
        histogram()->Fill(track->y(hit->position().z()), static_cast<TOFCluster*>(hit)->yEstimate());
  }
}

void TOFPositionCorrelationPlot::finalize()
{
}
