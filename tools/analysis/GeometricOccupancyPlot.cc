#include "GeometricOccupancyPlot.hh"
#include "BrokenLine.hh"

#include "Hit.hh"

#include <TH2.h>
#include <TAxis.h>

GeometricOccupancyPlot::GeometricOccupancyPlot(double zPosition)
  : AnalysisPlot(AnalysisPlot::Occupancy)
  , H2DPlot()
  , m_zPosition(zPosition)
{
  setTitle(QString("occupancy %1").arg(zPosition));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 100, -250, 250, 120, -120, 120);
  histogram->GetXaxis()->SetTitle("y / mm");
  histogram->GetYaxis()->SetTitle("x / mm");
  setHistogram(histogram);
}

GeometricOccupancyPlot::~GeometricOccupancyPlot()
{}

void GeometricOccupancyPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  int nTrackerHits = 0;
  foreach(Hit* hit, clusters)
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
    return;

  histogram()->Fill(track->y(m_zPosition), track->x(m_zPosition));
}

void GeometricOccupancyPlot::finalize()
{
}
