#include "BendingAnglePositionPlot.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include "Hit.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TCanvas.h>

#include <cmath>

BendingAnglePositionPlot::BendingAnglePositionPlot(double cut)
  : AnalysisPlot(AnalysisPlot::Tracking)
  , H2DPlot()
  , m_cut(cut)
  , m_normHisto(0)
{
  setTitle(QString("position of tracks with abs(alpha) > %1 rad").arg(cut));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 30, -250, 250, 24, -120, 120);
  histogram->GetXaxis()->SetTitle("y / mm");
  histogram->GetYaxis()->SetTitle("x / mm");
  setHistogram(histogram);
  m_normHisto = new TH2D(qPrintable(title()+"_all"), "", 30, -250, 250, 24, -120, 120);
  m_normHisto->GetXaxis()->SetTitle("y / mm");
  m_normHisto->GetYaxis()->SetTitle("x / mm");
}

BendingAnglePositionPlot::~BendingAnglePositionPlot()
{
  delete m_normHisto;
}

void BendingAnglePositionPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  histogram()->Draw("lego");
}

void BendingAnglePositionPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (!track)
    return;

  int nTrackerHits = 0;
  foreach(Hit* hit, clusters)
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
    return;

  double alpha = track->bendingAngle();

  if (alpha > m_cut)
    histogram()->Fill(track->y(0), track->x(0));

  m_normHisto->Fill(track->y(0), track->x(0));
}

void BendingAnglePositionPlot::finalize()
{
  histogram()->Divide(m_normHisto);
}
