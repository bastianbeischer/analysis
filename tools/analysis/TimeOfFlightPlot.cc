#include "TimeOfFlightPlot.hh"
#include "BrokenLine.hh"
#include "TrackInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"

#include <TH1.h>
#include <TVector3.h>
#include <TLatex.h>

#include <QDebug>

TimeOfFlightPlot::TimeOfFlightPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
{
  setTitle("time of flight");
  TH1D* histogram = new TH1D("time of flight", "", 1200, -60, 60);
  histogram->GetXaxis()->SetTitle("1 / #beta");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
  addLatex(RootPlot::newLatex(.15, .82));
}

TimeOfFlightPlot::~TimeOfFlightPlot()
{}

void TimeOfFlightPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;
  histogram()->Fill(1./track->beta());
}

void TimeOfFlightPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("mean = %1").arg(histogram()->GetMean())));
  latex(1)->SetTitle(qPrintable(QString("RMS  = %1").arg(histogram()->GetRMS())));
}
