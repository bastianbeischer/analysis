#include "BendingAnglePositionPlot.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include "Hit.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TCanvas.h>

#include <cmath>

BendingAnglePositionPlot::BendingAnglePositionPlot(double cut)
  : H2DPlot(AnalysisPlot::Tracking)
  , m_cut(cut)
{
  setTitle(QString("position of tracks with abs(alpha) > %1 rad").arg(cut));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 30, -250, 250, 24, -120, 120);
  histogram->GetXaxis()->SetTitle("y / mm");
  histogram->GetYaxis()->SetTitle("x / mm");
  setHistogram(histogram);
}

BendingAnglePositionPlot::~BendingAnglePositionPlot()
{}

void BendingAnglePositionPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  histogram()->Draw("lego");
}

void BendingAnglePositionPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
	if (!track)
		return;
  double upperSlope = 0, lowerSlope = 0;
  if (track->type() == Track::BrokenLine) {
    upperSlope = static_cast<BrokenLine*>(track)->upperSlopeX();
    lowerSlope = static_cast<BrokenLine*>(track)->lowerSlopeX();
  }
  if (track->type() == Track::CenteredBrokenLine) {
    upperSlope = static_cast<CenteredBrokenLine*>(track)->upperSlopeX();
    lowerSlope = static_cast<CenteredBrokenLine*>(track)->lowerSlopeX();
  }
  if (track->type() == Track::BrokenLine || track->type() == Track::CenteredBrokenLine) {
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    double alpha = atan(upperSlope) - atan(lowerSlope);
    if (alpha > m_cut)
			histogram()->Fill(track->y(0), track->x(0));
  }
}

void BendingAnglePositionPlot::finalize()
{
}
