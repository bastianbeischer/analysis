#include "GeometricOccupancyProjectionPlot.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"

#include "TrackSelection.hh"
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

void GeometricOccupancyProjectionPlot::processEvent(const QVector<Hit*>&, Track* track, TrackSelection* selection, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !selection || !track->fitGood())
    return;

  TrackSelection::Flags flags = selection->flags();
  if (!(flags & TrackSelection::AllTrackerLayers))
    return;

  histogram(0)->Fill(track->x(m_zPosition));
}
