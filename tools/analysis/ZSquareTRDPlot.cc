#include "ZSquareTRDPlot.hh"
#include "BrokenLine.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "TimeOfFlight.hh"
#include "Hit.hh"
#include "TOFCluster.hh"
#include "Constants.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>

#include <QDebug>

ZSquareTRDPlot::ZSquareTRDPlot()
  : AnalysisPlot(AnalysisPlot::SignalHeightTRD)
  , H2DPlot()
{
  setTitle(QString("z square TRD"));
  setAxisTitle("signal height upper TRD / ADC counts", "signal height lower TRD / ADC counts", "");
  TH2D* histogram = new TH2D(qPrintable(title()), "", 50, 0, 100, 50, 0, 100);
  addHistogram(histogram);
}

ZSquareTRDPlot::~ZSquareTRDPlot()
{
}

void ZSquareTRDPlot::processEvent(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good)))
    return;

  QVector<Hit*>::const_iterator endIt = clusters.end();
  int nUpperTrd = 0;
  int nLowerTrd = 0;
  double upperSum = 0.;
  double lowerSum = 0.;
  for (QVector<Hit*>::const_iterator it = clusters.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::trd) {
      if (hit->position().z() > -411.6) {
        ++nUpperTrd;
        upperSum+= hit->signalHeight();
      } else {
        ++nLowerTrd;
        lowerSum+= hit->signalHeight();
      }
    }
  }
  double s = Constants::upperTofPosition - Constants::lowerTofPosition;
  double c = Constants::speedOfLight;
  double t = particle->timeOfFlight()->timeOfFlight();
  double m = Constants::protonMass;
  double pCut = 1.5; //GeV
  if (2 < t && t < s / c * sqrt(pCut*pCut + m*m) / pCut)
  //if (track->rigidity() > pCut)
    histogram()->Fill(upperSum/nUpperTrd, lowerSum/nLowerTrd);
}
