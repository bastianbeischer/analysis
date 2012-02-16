#include "Chi2Plot.hh"

#include <TF1.h>
#include <TLatex.h>
#include <TH1D.h>
#include <TMath.h>

#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"

double chisquare(double* x, double* p)
{
  double amplitude = p[0];
  double ndf = p[1];
  double nom = pow(x[0],ndf/2. - 1.) * exp(-x[0]/2.);
  double denom = pow(2., ndf/2.) * TMath::Gamma(ndf/2.);
  return amplitude*nom/denom;
}

Chi2Plot::Chi2Plot(unsigned short ndf) :
  AnalysisPlot(Enums::Tracking),
  H1DPlot(),
  m_ndf(ndf)
{
  setTitle(QString("chi2 for ndf = %1").arg(m_ndf));
  int nBins = 100;
  double x0 = 0.;
  double x1 = 100.;

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, x0, x1);
  setAxisTitle("#chi^{2}", "");
  addHistogram(histogram);

  TF1* chi2function = new TF1(qPrintable(title() + " function"), chisquare, x0, x1, 2);
  chi2function->SetLineColor(kRed);
  chi2function->SetNpx(1000);
  chi2function->FixParameter(0, 1);
  chi2function->FixParameter(1, m_ndf);
  addFunction(chi2function);

  addLatex(RootPlot::newLatex(.55, .85));
  addLatex(RootPlot::newLatex(.55, .82));
}

Chi2Plot::~Chi2Plot()
{
}

void Chi2Plot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::AllTrackerLayers))
    return;
  if (track->ndf() == m_ndf)
    histogram()->Fill(track->chi2());
}

void Chi2Plot::update()
{
  latex(0)->SetTitle(qPrintable(QString("mean = %1").arg(histogram()->GetMean())));
  latex(1)->SetTitle(qPrintable(QString("RMS  = %1").arg(histogram()->GetRMS())));
}

void Chi2Plot::finalize()
{
  double integral = histogram()->Integral("WIDTH");
  if (!qIsNull(integral))
    histogram()->Scale(1./integral);
  yAxis()->SetRangeUser(0, 0.1);
}
