#include "ZenithAzimuthCorrelation.hh"

#include <TH2D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"

#include <iostream>
#include <cmath>

ZenithAzimuthCorrelation::ZenithAzimuthCorrelation()
  : AnalysisPlot(Enums::Tracking)
  , H2DPlot()
{
  setTitle("Zenith Azimuth Correlation");
  int nBinsAzmuth = 45;
  double xMinAzmuth = -180;
  double xMaxAzmuth = 180;
  int nBinsZenith = 200;
  double xMinZenith = 0;
  double xMaxZenith = 1;
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsAzmuth, xMinAzmuth, xMaxAzmuth, nBinsZenith, xMinZenith, xMaxZenith);
  histogram->Sumw2();
  setAxisTitle("azimuth / degree", "cos(zenith)", "");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.4, .4));
  int low = histogram->GetYaxis()->FindBin(0.8);
  int up = histogram->GetYaxis()->FindBin(1);
  histogram->GetYaxis()->SetRange(low, up);
  histogram->GetYaxis()->SetTitleOffset(1.2);
}

ZenithAzimuthCorrelation::~ZenithAzimuthCorrelation()
{
}

void ZenithAzimuthCorrelation::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(Enums::Chi2Good | Enums::AllTrackerLayers | Enums::InsideMagnet))
    return;
  if (event->flagsSet(Enums::Albedo))
    return;

  double xU = track->x(Constants::upperTofPosition);
  double yU = track->y(Constants::upperTofPosition);

  double xL = track->x(Constants::lowerTofPosition);
  double yL = track->y(Constants::lowerTofPosition);

  if (qAbs(yU) > Constants::tofBarLength / 2. || qAbs(xU) > Constants::tofBarWidth * 2.)
    return;
  if (qAbs(yL) > Constants::tofBarLength / 2. || qAbs(xL) > Constants::tofBarWidth * 2.)
    return;

  double azimuth = track->azimuthAngle() * 180. / M_PI;
  double zenith = track->zenithAngle();

  histogram()->Fill(azimuth, cos(zenith));
}

void ZenithAzimuthCorrelation::update()
{
  int value = histogram()->GetEntries();
  QString text = "entries: " + QString::number(value, 'd', 0);
  latex()->SetTitle(qPrintable(text));
}
