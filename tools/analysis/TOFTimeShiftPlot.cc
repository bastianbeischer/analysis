#include "TOFTimeShiftPlot.hh"
#include "BrokenLine.hh"
#include "TrackInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"

#include <TH1.h>
#include <TF1.h>
#include <TLatex.h>
#include <TVector3.h>
#include <iostream>
#include <QDebug>

double doubleGauss(double x[], double p[])
{
  return exp(-(x[0]-p[0])*(x[0]-p[0])/(p[1]*p[1]))*p[2] + exp(-(x[0]-p[0])*(x[0]-p[0])/(p[3]*p[3]))*p[4];
}

TOFTimeShiftPlot::TOFTimeShiftPlot(unsigned short topBarId, unsigned short bottomBarId, bool negativeSide)
  : AnalysisPlot(AnalysisPlot::CalibrationTOF)
  , H1DPlot()
  , m_topBarId(topBarId)
  , m_bottomBarId(bottomBarId)
  , m_negativeSide(negativeSide)
{
  QString title = QString("time shift 0x%1 0x%2 %3").arg(topBarId, 0, 16).arg(bottomBarId, 0, 16).arg(negativeSide ? 'n' : 'p');
  setTitle(title);
  for (int i = 1; i < 4; ++i) {
    TH1D* histogram = new TH1D(qPrintable(QString("%1 %2 histogram").arg(title).arg(i)), ";#Deltat / ns;", 100, -5, 5);
    histogram->SetLineColor(i);
    addHistogram(histogram);
    TF1* function = new TF1(qPrintable(QString("%1 %2 function").arg(title).arg(i)), doubleGauss, -5, 5, 5);
    function->SetLineColor(i);
    function->SetNpx(1000);
    function->SetParLimits(1, 0.3, 1.0);
    function->SetParLimits(3, 1.0, 5.0);
    addFunction(function);
    TLatex* latex = 0;
    latex = RootPlot::newLatex(.15, .92 - .10 * i);
    latex->SetTextColor(i);
    addLatex(latex);
    latex = RootPlot::newLatex(.15, .89 - .10 * i);
    latex->SetTextColor(i);
    addLatex(latex);
    latex = RootPlot::newLatex(.15, .86 - .10 * i);
    latex->SetTextColor(i);
    addLatex(latex);
  }
}

TOFTimeShiftPlot::~TOFTimeShiftPlot()
{}

void TOFTimeShiftPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;
  double t[4];
  for (int i = 0; i < 4; ++i)
    t[i] = -2e6;
  foreach (Hit* hit, hits) {
    if (hit->detId() == m_topBarId || hit->detId() == m_bottomBarId) {
      TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      const TVector3& position = cluster->position();
      if (qAbs(track->x(position.z())-position.x()) > 10)
        continue;
      foreach (Hit* sipmHit, cluster->hits()) {
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(sipmHit);
        if ((tofSipmHit->position().y() < 0) == m_negativeSide) {
          if (m_negativeSide && (track->y(position.z()) < -150 || track->y(position.z()) > 0))
            continue;
          if (!m_negativeSide && (track->y(position.z()) > 150 || track->y(position.z()) < 0))
            continue;
          int tofChannel = tofSipmHit->detId() - cluster->detId();
          int offset = m_negativeSide ? 0 : -2;
          if (cluster->detId() == m_bottomBarId)
            offset += 2;
          t[offset+tofChannel] = tofSipmHit->startTime();
        }
      }
    }
  }
  for (int i = 1; i < 4; ++i) {
    if (t[0] < -1e6)
      return;
    if (t[i] > -1e6)
      histogram(i-1)->Fill(t[i]-t[0]);
  }
}

void TOFTimeShiftPlot::update()
{
  for (int i = 0; i < 3; ++i) {
    latex(3*i+0)->SetTitle(qPrintable(QString("n    = %1").arg(histogram(i)->GetEntries())));
    latex(3*i+1)->SetTitle(qPrintable(QString("mean = %1 ns").arg(histogram(i)->GetMean(), 0, 'f', 2, ' ')));
    double max = histogram(i)->GetMaximum();
    double mean = histogram(i)->GetMean();
    double sigma = histogram(i)->GetRMS();
    function(i)->SetParameters(mean, sigma, 0.9*max, 3.*sigma, 0.1*max);
    histogram(i)->Fit(function(i), "QN0");
    latex(3*i+2)->SetTitle(qPrintable(QString("#mu    = %1 ns").arg(function(i)->GetParameter(1), 0, 'f', 2, ' ')));
  }
  H1DPlot::update();
}
