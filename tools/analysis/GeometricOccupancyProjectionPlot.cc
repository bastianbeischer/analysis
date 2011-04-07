#include "GeometricOccupancyProjectionPlot.hh"
#include "Particle.hh"
#include "Track.hh"

#include "ParticleInformation.hh"
#include "Hit.hh"

#include <TH1.h>
#include <TCanvas.h>

#include <cmath>

GeometricOccupancyProjectionPlot::GeometricOccupancyProjectionPlot(double zPosition)
  : AnalysisPlot(AnalysisPlot::Occupancy)
  , H1DPlot()
  , m_zPosition(zPosition)
{
  setTitle(QString("occupancy projection %1").arg(zPosition));
  TH1D* histogram = new TH1D(qPrintable(title()), "", 240, -120, 120);
  setAxisTitle("x / mm", "");
  addHistogram(histogram);
}

GeometricOccupancyProjectionPlot::~GeometricOccupancyProjectionPlot()
{}

void GeometricOccupancyProjectionPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  histogram()->Fill(track->x(m_zPosition));
}
