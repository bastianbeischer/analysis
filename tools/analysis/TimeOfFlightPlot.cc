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
  TLatex* latex = 0;
  latex = new TLatex(.15, .85, 0);
  latex->SetNDC();
  latex->SetTextAlign(13);
  latex->SetTextFont(82);
  latex->SetTextSize(0.03);
  addLatex(latex);
  latex = new TLatex(.15, .82, 0);
  latex->SetNDC();
  latex->SetTextAlign(13);
  latex->SetTextFont(82);
  latex->SetTextSize(0.03);
  addLatex(latex);
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

  double deltaT = track->timeOfFlight();

  const TVector3& upperPoint = track->position(upperTofPosition);
  const TVector3& lowerPoint = track->position(lowerTofPosition);
  double trackLength = (upperPoint-lowerPoint).Mag();

  histogram()->Fill(1./track->beta());
}

void TimeOfFlightPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("mean = %1").arg(histogram()->GetMean())));
  latex(1)->SetTitle(qPrintable(QString("RMS  = %1").arg(histogram()->GetRMS())));
}
