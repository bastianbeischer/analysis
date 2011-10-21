#include "AzimuthMigrationHistogram.hh"

#include <TH2D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"

#include <QDebug>

#include <iostream>
#include <cmath>

AzimuthMigrationHistogram::AzimuthMigrationHistogram() :
AnalysisPlot(Enums::MonteCarloTracker),
H2DPlot()
{
  setTitle("Azimuth migration");
  const int nBinsGenerated = 20;
  const double minGenerated = -180;
  const double maxGenerated = 180;
  const int nBinsReconstructed = 45;
  const double minReconstructed = -180;
  const double maxReconstructed = 180;
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsReconstructed, minReconstructed, maxReconstructed, nBinsGenerated, minGenerated, maxGenerated);
  histogram->Sumw2();
  setAxisTitle("reconstructed azimuth", "generated azimuth", "");
  addHistogram(histogram);
}

AzimuthMigrationHistogram::~AzimuthMigrationHistogram()
{
}

void AzimuthMigrationHistogram::processEvent(const AnalyzedEvent* event)
{
  if (event->simpleEvent()->contentType() != SimpleEvent::MonteCarlo)
    return;
  if (event->simpleEvent()->MCInformation()->primary()->initialMomentum.Z() > 0)
    return;
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet | ParticleInformation::Albedo))
    return;
  if (!event->simpleEvent()->MCInformation()->primary()->isInsideMagnet())
    return;

  double azimuthGenerated = event->simpleEvent()->MCInformation()->primary()->azimuthAngle() * 180. / M_PI;
  double azimuthReconstructed = (track->azimuthAngle()) * 180. / M_PI;

  if (azimuthGenerated == 180.)
    azimuthGenerated = -180.;
  if (azimuthReconstructed == 180.)
    azimuthReconstructed = -180.;

  histogram()->Fill(azimuthReconstructed, azimuthGenerated);
  Q_ASSERT(180. >= qAbs(azimuthGenerated));
  Q_ASSERT(180. >= qAbs(azimuthReconstructed));
}
