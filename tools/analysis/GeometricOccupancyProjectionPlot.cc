#include "GeometricOccupancyProjectionPlot.hh"
#include "Track.hh"

#include "TrackInformation.hh"
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
  histogram->GetXaxis()->SetTitle("x / mm");
  addHistogram(histogram);
}

GeometricOccupancyProjectionPlot::~GeometricOccupancyProjectionPlot()
{}

void GeometricOccupancyProjectionPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  histogram()->Fill(track->x(m_zPosition));
}
