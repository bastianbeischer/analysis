#include "Chi2Plot.hh"

#include <TF1.h>
#include <TH1D.h>
#include <TMath.h>

#include "TrackInformation.hh"
#include "Hit.hh"
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
  AnalysisPlot(AnalysisPlot::Tracking),
  H1DPlot(),
  m_ndf(ndf)
{
  setTitle(QString("chi2 for ndf = %1").arg(m_ndf));
  int nBins = 100;
  double x0 = 0.;
  double x1 = 100.;

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, x0, x1);
  histogram->GetXaxis()->SetTitle("#chi^{2}");
  histogram->GetYaxis()->SetTitle("entries");
  addHistogram(histogram);

  TF1* chi2function = new TF1("chi2function", chisquare, x0, x1, 2);
  chi2function->SetLineColor(kRed);
  chi2function->SetNpx(1000);
  chi2function->FixParameter(0, 1);
  chi2function->FixParameter(1, ndf);
  addFunction(chi2function);
}

Chi2Plot::~Chi2Plot()
{
}

void Chi2Plot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent* /*event*/)
{
  // QMutexLocker locker(&m_mutex);
  if(!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  if (track->ndf() == m_ndf)
    histogram()->Fill(track->chi2());
}

void Chi2Plot::finalize()
{
  histogram()->Scale(1./histogram()->Integral("WIDTH"));
  histogram()->GetYaxis()->SetRangeUser(0, 0.1);
}
