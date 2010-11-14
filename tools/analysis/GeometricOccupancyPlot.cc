#include "GeometricOccupancyPlot.hh"
#include "BrokenLine.hh"

#include "Hit.hh"

#include <TH2.h>
#include <TAxis.h>

GeometricOccupancyPlot::GeometricOccupancyPlot(double zPosition)
  : H2DPlot(AnalysisPlot::Occupancy, QString("occupancy %1").arg(zPosition), 100, -250, 250, 120, -120, 120)
  , m_zPosition(zPosition)
{
  m_histogram->GetXaxis()->SetTitle("y / mm");
  m_histogram->GetYaxis()->SetTitle("x / mm");
}

GeometricOccupancyPlot::~GeometricOccupancyPlot()
{}

void GeometricOccupancyPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (track) {
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    m_histogram->Fill(track->y(m_zPosition), track->x(m_zPosition));
  }
}

void GeometricOccupancyPlot::finalize()
{
}
