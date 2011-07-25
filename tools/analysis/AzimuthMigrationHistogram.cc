#include "AzimuthMigrationHistogram.hh"

#include <TH2D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"

#include <iostream>
#include <cmath>

AzimuthMigrationHistogram::AzimuthMigrationHistogram() :
AnalysisPlot(AnalysisPlot::MonteCarloTracker),
H2DPlot()
{
  setTitle("Azimuth migration");
  const int nBinsGenerated = 45;
  const double minGenerated = -180;
  const double maxGenerated = 180;
  const int nBinsReconstructed = 90;
  const double minReconstructed = -180;
  const double maxReconstructed = 180;
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsReconstructed, minReconstructed, maxReconstructed, nBinsGenerated, minGenerated, maxGenerated);
  histogram->Sumw2();
  setAxisTitle("reconstructed azimuth", "generated azimuth", "entries");
  addHistogram(histogram);
}

AzimuthMigrationHistogram::~AzimuthMigrationHistogram()
{
}

void AzimuthMigrationHistogram::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  if (event->MCInformation()->primary()->initialMomentum.Z() > 0)
    return;
  
  const Track* track = particle->track();
  
  if (!track || !track->fitGood())
    return;
  
  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::AllTrackerLayers) || !(flags & ParticleInformation::InsideMagnet) || (flags & ParticleInformation::Albedo) )
    return;
  
  double azimuthGenerated = azimuthAngle(event->MCInformation()->primary()->initialMomentum) / M_PI * 180;
  double azimuthReconstructed = (track->azimuthAngle()) / M_PI * 180;
  
  histogram()->Fill(azimuthReconstructed, azimuthGenerated);
  if (azimuthGenerated <= -180 || 180 <= azimuthGenerated )
    qDebug("generated smaler lager than abs(180)");
  if (azimuthReconstructed <= -180 || 180 <= azimuthReconstructed )
    qDebug("reconstructed smaler lager than abs(180)");
}

void AzimuthMigrationHistogram::update()
{
  
}

double AzimuthMigrationHistogram::azimuthAngle(const TVector3& initialMomentum)
{
  //with respect to the x-axis
  const double y = (-1*initialMomentum).Y();
  const double x = (-1*initialMomentum).X();
  
  if (x > 0) {
    return atan(y / x);
  } else if (x < 0) {
    if (y >= 0) {
      return atan(y / x) + M_PI;
    } else {
      return atan(y / x) - M_PI;
    }
  } else {
    if (y > 0) {
      return M_PI / 2.;
    } else if (y < 0) {
      return -M_PI / 2.;
    } else {
      return NAN;
    }
  }
}