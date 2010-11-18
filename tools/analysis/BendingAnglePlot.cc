#include "BendingAnglePlot.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include "Hit.hh"
#include "TrackSelection.hh"

#include <TH1.h>
#include <TCanvas.h>

#include <cmath>

BendingAnglePlot::BendingAnglePlot()
  : AnalysisPlot(AnalysisPlot::Tracking)
  , H1DPlot()
{
  setTitle("bending angle");
  TH1D* histogram = 0;
  histogram = new TH1D("bending angle all tracks", "", 400, -.2, .2);
  histogram->GetXaxis()->SetTitle("bending angle / rad");
  addHistogram(histogram);
  histogram = new TH1D("bending angle insige magnet", "", 400, -.2, .2);
  histogram->SetLineColor(kRed);
  addHistogram(histogram);
  histogram = new TH1D("bending angle outside magnet", "", 400, -.2, .2);
  histogram->SetLineColor(kBlue);
  addHistogram(histogram);
}

BendingAnglePlot::~BendingAnglePlot()
{}

void BendingAnglePlot::processEvent(const QVector<Hit*>& /*clusters*/, Track* track, TrackSelection* selection, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !selection || !track->fitGood())
    return;

  TrackSelection::Flags flags = selection->flags();
  if (!(flags & TrackSelection::AllTrackerLayers))
    return;

  double alpha = track->bendingAngle();

  histogram(0)->Fill(alpha);
  if(flags & TrackSelection::InsideMagnet)
    histogram(1)->Fill(alpha);
  if(flags & TrackSelection::OutsideMagnet)
    histogram(2)->Fill(alpha);
}

void BendingAnglePlot::finalize()
{
  histogram(0)->Scale(1./histogram(0)->Integral("width"));
  histogram(1)->Scale(1./histogram(1)->Integral("width"));
  histogram(2)->Scale(1./histogram(2)->Integral("width"));
}
