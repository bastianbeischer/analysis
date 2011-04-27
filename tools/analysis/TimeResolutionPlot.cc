#include "TimeResolutionPlot.hh"
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
#include "TimeOfFlight.hh"

#include <TH2.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TF1.h>

#include <QDebug>

TimeResolutionPlot::TimeResolutionPlot(unsigned short idTop1, unsigned short idTop2, unsigned short idBottom1, unsigned short idBottom2)
  : AnalysisPlot(AnalysisPlot::ResolutionTOF)
  , H2DPlot()
  , m_idTop1(idTop1)
  , m_idTop2(idTop2)
  , m_idBottom1(idBottom1)
  , m_idBottom2(idBottom2)
{
  QString title = QString("time resolution 0x%1 0x%2 0x%3 0x%4")
    .arg(m_idTop1, 0, 16)
    .arg(m_idTop2, 0, 16)
    .arg(m_idBottom1, 0, 16)
    .arg(m_idBottom2, 0, 16);
  setTitle(title);
  TH2D* histogram = new TH2D(qPrintable(title), "",
    5, -Constants::tofBarLength / 2., Constants::tofBarLength / 2., 30, 0, 6);
  setAxisTitle("y / mm", "#Deltat / ns", "");
  addHistogram(histogram);
}

TimeResolutionPlot::~TimeResolutionPlot()
{}

void TimeResolutionPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();
  const TimeOfFlight* tof = particle->timeOfFlight();

  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
    return;
  if (track->rigidity() < 1)
    return;
  bool idTop1 = false, idTop2 = false, idBottom1 = false, idBottom2 = false;
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    unsigned short barId = hit->detId() - hit->channel();
    if (!idTop1 && barId == m_idTop1) idTop1 = true;
    if (!idTop2 && barId == m_idTop2) idTop2 = true;
    if (!idBottom1 && barId == m_idBottom1) idBottom1 = true;
    if (!idBottom2 && barId  == m_idBottom2) idBottom2 = true;
  }
  if (idTop1 && idTop2 && idBottom1 && idBottom2) {
    double l = track->trackLength();
    double d = Constants::upperTofPosition - Constants::lowerTofPosition;
    double lCorrection = (d - l) / Constants::speedOfLight;
    double m = Constants::protonMass; //TODO: use reconstructed particle
    double rigidity = track->rigidity();
    double t = tof->timeOfFlight();
    double pCorrection = (t + lCorrection) * (1 - sqrt(rigidity*rigidity + m*m) / rigidity);
    double yu = track->y(Constants::upperTofPosition);
    double yl = track->y(Constants::lowerTofPosition);
    double binWidth = xAxis()->GetBinWidth(1);
    if (qAbs(yu - yl) < binWidth)
      histogram()->Fill((yu + yl) / 2., t + lCorrection + pCorrection);
  }
}
