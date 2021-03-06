#include "Chi2PerNdfPlot.hh"

#include <TLatex.h>
#include <TH1D.h>

#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"

Chi2PerNdfPlot::Chi2PerNdfPlot() :
  AnalysisPlot(Enums::Tracking),
  H1DPlot()
{
  setTitle("chi2 / ndf");
  int nBins = 100;
  double x0 = 0.;
  double x1 = 10.;

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, x0, x1);
  setAxisTitle("#chi^{2} / ndf", "");
  addHistogram(histogram);

  addLatex(RootPlot::newLatex(.55, .85));
  addLatex(RootPlot::newLatex(.55, .82));
}

Chi2PerNdfPlot::~Chi2PerNdfPlot()
{
}

void Chi2PerNdfPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const /*event*/)
{
  const Track* track = particle->track();

  if(!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  histogram()->Fill(track->chi2() / track->ndf());
}

void Chi2PerNdfPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("mean = %1").arg(histogram()->GetMean())));
  latex(1)->SetTitle(qPrintable(QString("RMS  = %1").arg(histogram()->GetRMS())));
}
