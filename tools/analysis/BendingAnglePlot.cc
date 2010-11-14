#include "BendingAnglePlot.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include "Hit.hh"

#include <TH1.h>
#include <TCanvas.h>

#include <cmath>

BendingAnglePlot::BendingAnglePlot()
  : H1DPlot(AnalysisPlot::Tracking, "bending angle", 2000, -.2, .2)
	, m_insideMagnetHistogram(new TH1D("bending angle insige magnet", "", 400, -.2, .2))
	, m_outsideMagnetHistogram(new TH1D("bending angle outside magnet", "", 400, -.2, .2))
{
  m_histogram->GetXaxis()->SetTitle("bending angle / rad");
	m_insideMagnetHistogram->SetLineColor(kRed);
	m_outsideMagnetHistogram->SetLineColor(kBlue);
}

BendingAnglePlot::~BendingAnglePlot()
{}

void BendingAnglePlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
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
		double r = track->x(0)*track->x(0) + track->y(0)*track->y(0);
    //m_histogram->Fill(alpha);
		if (r < 150)
			m_insideMagnetHistogram->Fill(alpha);
		if (r > 250) {
			m_outsideMagnetHistogram->Fill(alpha);
			m_histogram->Fill(alpha);
		}

  }
}

void BendingAnglePlot::finalize()
{
}

void BendingAnglePlot::draw(TCanvas* canvas) const
{
	H1DPlot::draw(canvas);
	m_insideMagnetHistogram->Draw("SAME");
	m_outsideMagnetHistogram->Draw("SAME");
}
