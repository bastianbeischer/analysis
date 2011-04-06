#include "GeometricOccupancyPlot.hh"
#include "BrokenLine.hh"

#include "TrackInformation.hh"
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
  setAxisTitle("y / mm", "x / mm", "");
  addHistogram(histogram);
}

GeometricOccupancyPlot::~GeometricOccupancyPlot()
{}

void GeometricOccupancyPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  histogram()->Fill(track->y(m_zPosition), track->x(m_zPosition));
}
