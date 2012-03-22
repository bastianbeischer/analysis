#include "BendingAnglePlot.hh"
#include "Particle.hh"
#include "Track.hh"

#include "Hit.hh"
#include "ParticleInformation.hh"

#include <TH1.h>
#include <TCanvas.h>

#include <cmath>

BendingAnglePlot::BendingAnglePlot()
  : AnalysisPlot(Enums::Tracking)
  , H1DPlot()
{
  setTitle("bending angle");
  setAxisTitle("bending angle / rad", "");
  TH1D* histogram = 0;
  histogram = new TH1D("bending angle all tracks", "", 400, -.2, .2);
  addHistogram(histogram);
  histogram = new TH1D("bending angle insige magnet", "", 400, -.2, .2);
  histogram->SetLineColor(kRed);
  addHistogram(histogram);
  histogram = new TH1D("bending angle outside magnet", "", 400, -.2, .2);
  histogram->SetLineColor(kBlue);
  addHistogram(histogram);
  
  addRequiredEventFlags(Enums::TrackGood | Enums::AllTrackerLayers);
}

BendingAnglePlot::~BendingAnglePlot()
{}

void BendingAnglePlot::processEvent(const AnalyzedEvent* event)
{
  double alpha = event->particle()->track()->bendingAngle();
  histogram(0)->Fill(alpha);
  if(event->flagsSet(Enums::InsideMagnet))
    histogram(1)->Fill(alpha);
  if(event->flagsSet(Enums::OutsideMagnet))
    histogram(2)->Fill(alpha);
}

void BendingAnglePlot::finalize()
{
  histogram(0)->Scale(1./histogram(0)->Integral("width"));
  histogram(1)->Scale(1./histogram(1)->Integral("width"));
  histogram(2)->Scale(1./histogram(2)->Integral("width"));
}
