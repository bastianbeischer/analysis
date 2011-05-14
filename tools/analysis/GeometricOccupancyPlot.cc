#include "GeometricOccupancyPlot.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ProjectionControlWidget.hh"

#include <QSpinBox>

#include <TH2.h>
#include <TAxis.h>

GeometricOccupancyPlot::GeometricOccupancyPlot(double zPosition)
  : AnalysisPlot(AnalysisPlot::Occupancy)
  , H2DProjectionPlot()
  , m_zPosition(zPosition)
{
  controlWidget()->spinBox()->setMaximum(1024);
  setTitle(QString("occupancy %1").arg(zPosition));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 100, -250, 250, 120, -120, 120);
  setAxisTitle("y / mm", "x / mm", "");
  addHistogram(histogram);
}

GeometricOccupancyPlot::~GeometricOccupancyPlot()
{}

void GeometricOccupancyPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  histogram()->Fill(track->y(m_zPosition), track->x(m_zPosition));
}
