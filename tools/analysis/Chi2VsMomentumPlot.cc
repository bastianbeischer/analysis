#include "Chi2VsMomentumPlot.hh"

#include <cmath>

#include <TH1D.h>
#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Hypothesis.hh"

Chi2VsMomentumPlot::Chi2VsMomentumPlot() :
  AnalysisPlot(Enums::Tracking),
  H2DPlot()
{
  setTitle("Chi2 vs Momentum");

  int nBinsX = 30;
  double x0 = 0.;
  double x1 = 10.;
  int nBinsY = 100.;
  double y0 = 0.;
  double y1 = 10.;

  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsX, x0, x1, nBinsY, y0, y1);
  setAxisTitle("R / GV", "#chi^{2} / ndf", "");
  addHistogram(histogram);

  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nBinsX, x0, x1);
}

Chi2VsMomentumPlot::~Chi2VsMomentumPlot()
{
  delete m_normHisto;
}

void Chi2VsMomentumPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(Enums::AllTrackerLayers))
    return;
  histogram()->Fill(event->particle()->hypothesis()->rigidity(), track->chi2() / track->ndf());
  m_normHisto->Fill(event->particle()->hypothesis()->rigidity());
}

void Chi2VsMomentumPlot::finalize()
{
  for (int i = 1; i <= histogram()->GetNbinsX(); i++) {
    for (int j = 1; j <= histogram()->GetNbinsY(); j++) {
      double bc = histogram()->GetBinContent(i,j);
      histogram()->SetBinContent(i,j,bc/m_normHisto->GetBinContent(i));
    }
  }
}
