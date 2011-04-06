#include "CutStatisticsPlot.hh"

#include <TH1D.h>
#include <TLatex.h>

#include "Particle.hh"
#include "Track.hh"
#include "TrackInformation.hh"

CutStatisticsPlot::CutStatisticsPlot() :
  AnalysisPlot(AnalysisPlot::MiscellaneousTracker),
  H1DPlot()
{
  setTitle("cut statistics");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 8, -0.5, 7.5);
  //histogram->GetXaxis()->SetNdivisions(8);
  //histogram->GetYaxis()->SetNdivisions(520);
  //histogram->SetMinimum(0.);
  setAxisTitle("cut number", "relative entries after cut");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(0., 0.));
  addLatex(RootPlot::newLatex(1., 0.));
  addLatex(RootPlot::newLatex(2., 0.));
  addLatex(RootPlot::newLatex(3., 0.));
  addLatex(RootPlot::newLatex(4., 0.));
  latex(0)->SetTitle("all triggered events");
  latex(1)->SetTitle("enough hits for fit");
  latex(2)->SetTitle("in inner acceptance");
  latex(3)->SetTitle("exactly one hit in each layer");
  latex(4)->SetTitle("going downward");
}

CutStatisticsPlot::~CutStatisticsPlot()
{
}

void CutStatisticsPlot::processEvent(const QVector<Hit*>& /*hits*/, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  histogram()->Fill(0);
  if (!track || !track->fitGood())
    return;
  histogram()->Fill(1);
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::InsideMagnet))
    return;
  histogram()->Fill(2);
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;
  histogram()->Fill(3);
  if ((flags & TrackInformation::Albedo))
    return;
  histogram()->Fill(4);
}

void CutStatisticsPlot::update()
{
  for (int i = 0; i < 5; i++) {
    latex(i)->SetNDC(false);
    latex(i)->SetX(i+0.6);
    latex(i)->SetY(histogram(0)->GetBinContent(i+1) + 0.02);
  }
}

void CutStatisticsPlot::finalize()
{
  histogram(0)->Scale(1./histogram()->GetBinContent(3));
  yAxis()->SetRangeUser(0., 1.05 * histogram()->GetMaximum());
}
