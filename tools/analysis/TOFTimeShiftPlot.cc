#include "TOFTimeShiftPlot.hh"
#include "BrokenLine.hh"
#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"
#include "Particle.hh"
#include "Track.hh"

#include <TH2.h>
#include <TVector3.h>
#include <iostream>
#include <QDebug>

TOFTimeShiftPlot::TOFTimeShiftPlot(unsigned short topBarId, unsigned short bottomBarId, unsigned short referenceChannel)
  : AnalysisPlot(AnalysisPlot::CalibrationTOF)
  , H2DProjectionPlot()
  , m_topBarId(topBarId)
  , m_bottomBarId(bottomBarId)
  , m_referenceChannel(referenceChannel)
{
  QString title = QString("time shift 0x%1 0x%2 ch 0x%3").arg(topBarId, 0, 16).arg(bottomBarId, 0, 16).arg(referenceChannel, 0, 16);
  setTitle(title);
  int nBins = 100;
  double min = -10;
  double max = 10;
  TH2D* histogram = new TH2D(qPrintable(title), "", 8, -0.5, 7.5, nBins, min-.5*(max-min)/nBins, max-.5*(max-min)/nBins);
  for (unsigned int ch = 0; ch < 8; ++ch) {
    unsigned int id = (ch < 4) ? (topBarId | ch) : (bottomBarId | (ch - 4));
    histogram->GetXaxis()->SetBinLabel(ch + 1, qPrintable(QString("0x%4").arg(id, 0, 16)));
  }
  setAxisTitle("channel", "#Deltat / ns", "");
  addHistogram(histogram);
}

TOFTimeShiftPlot::~TOFTimeShiftPlot()
{}

void TOFTimeShiftPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;
  
  const QVector<Hit*>& hits = track->hits();

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;
  double t[8];
  for (int i = 0; i < 8; ++i)
    t[i] = -1;
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->detId() == m_topBarId || hit->detId() == m_bottomBarId) {
      TOFCluster* cluster = static_cast<TOFCluster*>(hit);
      const TVector3& position = cluster->position();
      if (qAbs(track->y(position.z())) > 10)
        continue;
      if (qAbs(track->x(position.z())-position.x()) > 15)
        continue;
      foreach(Hit* hit, cluster->hits()) {
        TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
        int tofChannel = tofHit->detId() - cluster->detId();
        t[(m_topBarId == cluster->detId() ? 0 : 4) + tofChannel] = tofHit->startTime();
      }
    }
  }

  for (int ch = 0; ch < 8; ++ch)
    if (t[ch] < 0)
      return;

  for (unsigned int ch = 0; ch < 8; ++ch) {
    if (ch != m_referenceChannel)
      histogram()->Fill(ch, t[ch]-t[m_referenceChannel]);
  }
}
