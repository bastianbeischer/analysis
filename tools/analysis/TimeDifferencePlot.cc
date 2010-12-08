#include "TimeDifferencePlot.hh"
#include "BrokenLine.hh"
#include "TrackSelection.hh"
#include "Hit.hh"
#include "TOFConstants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"

#include <TH2.h>
#include <TVector3.h>

#include <QDebug>

TimeDifferencePlot::TimeDifferencePlot(unsigned short topBarId, unsigned short bottomBarId)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H2DPlot()
  , m_topBarId(topBarId)
  , m_bottomBarId(bottomBarId)
{
  setTitle("time difference");
  TH2D* histogram = new TH2D("time difference", "", 8, 0, 8, 100, -10, 10);
  histogram->GetXaxis()->SetTitle("channel");
  histogram->GetYaxis()->SetTitle("#Deltat / ns");
  setHistogram(histogram);
}

TimeDifferencePlot::~TimeDifferencePlot()
{}

void TimeDifferencePlot::processEvent(const QVector<Hit*>& hits, Track* track, TrackSelection* selection, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !selection || !track->fitGood())
    return;

  TrackSelection::Flags flags = selection->flags();
  if (!(flags & TrackSelection::AllTrackerLayers))
    return;

  foreach (Hit* hit, hits) {
    if (!strcmp(hit->ClassName(), "TOFCluster")) {
      //TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      //TOFBar* element = static_cast<TOFBar*>(Setup::instance()->element(cluster->detId()));
    }
  }

  //histogram()->Fill(deltaT * speedOfLight / trackLength);
}

void TimeDifferencePlot::finalize()
{}
