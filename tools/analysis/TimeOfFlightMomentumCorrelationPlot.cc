#include "TimeOfFlightMomentumCorrelationPlot.hh"
#include "BrokenLine.hh"

#include "TrackInformation.hh"
#include "Hit.hh"
#include "TOFCluster.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>

#include <QDebug>

TimeOfFlightMomentumCorrelationPlot::TimeOfFlightMomentumCorrelationPlot()
  : AnalysisPlot(AnalysisPlot::MomentumReconstruction)
  , H2DPlot()
{
  setTitle(QString("tof momentum correlation"));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 2000, -10, 10, 4000, -20, 20);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("1 / #beta");
  setHistogram(histogram);
}

TimeOfFlightMomentumCorrelationPlot::~TimeOfFlightMomentumCorrelationPlot()
{
}

void TimeOfFlightMomentumCorrelationPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & (TrackInformation::AllTrackerLayers | TrackInformation::InsideMagnet)))
    return;
  histogram()->Fill(track->p(), 1./track->beta());
}

void TimeOfFlightMomentumCorrelationPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  histogram()->Draw();
}

void TimeOfFlightMomentumCorrelationPlot::finalize()
{
}
