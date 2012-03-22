#include "AzimuthDistributionPlot.hh"

#include <QDebug>

#include <TH1D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"

#include <iostream>
#include <cmath>

AzimuthDistributionPlot::AzimuthDistributionPlot()
  : AnalysisPlot(Enums::Tracking)
  , H1DPlot()
{
  setTitle("Azimuth distribution");
  int nBins = 45;
  double xMin = -180;
  double xMax = 180;
  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, xMin, xMax);
  histogram->Sumw2();
  setAxisTitle("azimuth / degree", "");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));

  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good | Enums::InsideMagnet | Enums::AllTrackerLayers);
  addRequiredEventFlagsAbsence(Enums::Albedo);
}

AzimuthDistributionPlot::~AzimuthDistributionPlot()
{
}

void AzimuthDistributionPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();

  double xU = track->x(Constants::upperTofPosition);
  double yU = track->y(Constants::upperTofPosition);

  double xL = track->x(Constants::lowerTofPosition);
  double yL = track->y(Constants::lowerTofPosition);

  if (qAbs(yU) > Constants::tofBarLength/2. || qAbs(xU) > Constants::tofBarWidth*2.)
    return;
  if (qAbs(yL) > Constants::tofBarLength/2. || qAbs(xL) > Constants::tofBarWidth*2.)
    return;

  double azimuth = track->azimuthAngle() * 180. / M_PI;

  histogram()->Fill(azimuth);
}
