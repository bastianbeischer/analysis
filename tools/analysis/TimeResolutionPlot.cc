#include "TimeResolutionPlot.hh"
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
#include <TF1.h>

#include <QDebug>

TimeResolutionPlot::TimeResolutionPlot(unsigned short idTop1, unsigned short idTop2, unsigned short idBottom1, unsigned short idBottom2)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
  , m_idTop1(idTop1)
  , m_idTop2(idTop2)
  , m_idBottom1(idBottom1)
  , m_idBottom2(idBottom2)
  , m_y(0)
{
  QString title = QString("time resolution 0x%1 0x%2 0x%3 0x%4")
    .arg(m_idTop1, 0, 16)
    .arg(m_idTop2, 0, 16)
    .arg(m_idBottom1, 0, 16)
    .arg(m_idBottom2, 0, 16);
  setTitle(title);
  TH1D* histogram = new TH1D(qPrintable(title + " all histogram"), "", 30, 0, 6);
  histogram->GetXaxis()->SetTitle("#Deltat / ns");
  addHistogram(histogram);
  TF1* function = new TF1(qPrintable(title + " all function"), "gaus(0)", 0, 6);
  addFunction(function);

  qDebug()
    << Setup::instance()->element(m_idTop1)->position().x()
    << Setup::instance()->element(m_idTop2)->position().x()
    << Setup::instance()->element(m_idBottom1)->position().x()
    << Setup::instance()->element(m_idBottom2)->position().x();

  addLatex(RootPlot::newLatex(.15, .85));
  addLatex(RootPlot::newLatex(.15, .82));
  addLatex(RootPlot::newLatex(.15, .79));
}

TimeResolutionPlot::~TimeResolutionPlot()
{}

void TimeResolutionPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;
  if (track->p() < 3)
    return;
  if (qAbs(track->y(upperTofPosition)-m_y) < 150. && qAbs(track->y(lowerTofPosition)-m_y) < 150.) {
    histogram()->Fill(track->timeOfFlight());
    unsigned short up = 0;
    unsigned short down = 0;
    foreach (Hit* hit, hits) {
      if (hit->type() != Hit::tof)
        continue;
      TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      const TVector3& position = cluster->position();
      if (qAbs(track->x(position.z())-position.x()) > 25)
        continue;
      if (position.z() > 0) {
        //Q_ASSERT(up == 0);
        up = cluster->detId();
      } else {
        //Q_ASSERT(down == 0);
        down = cluster->detId();
      }
    }
    /*for (int i = 0; i < m_upperId.count(); ++i) {
      if (m_upperId[i] == up && m_lowerId[i] == down)
        histogram(i+1)->Fill((upperTofPosition-lowerTofPosition)/(speedOfLight*track->beta()));
    }*/
  }
}

void TimeResolutionPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("mean  = %1").arg(histogram()->GetMean())));
  latex(1)->SetTitle(qPrintable(QString("RMS   = %1").arg(histogram()->GetRMS())));
}

void TimeResolutionPlot::finalize()
{
  function()->SetParameters(histogram()->GetMaximum(), histogram()->GetMean(), histogram()->GetRMS());
  histogram()->Fit(function(), "RQN0");
  latex(2)->SetTitle(qPrintable(QString("sigma = %1").arg(function()->GetParameter(2))));
}
