#include "Cut.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "Enums.hh"

#include <cmath>

#include <QDebug>
#include <QStringList>

Cut::Cut(Enums::Cut type)
  : m_type(type)
  , m_minIsSet(false)
  , m_maxIsSet(false)
{
}

Cut::Cut(const QString& string)
  : m_minIsSet(false)
  , m_maxIsSet(false)
{
  QStringList stringList = string.split(" | ");
  m_type = Enums::cut(stringList[0]);
  if (!stringList[1].isEmpty())
    setMin(stringList[1].toDouble());
  if (!stringList[2].isEmpty())
    setMax(stringList[2].toDouble());
}

Cut::~Cut()
{
}

void Cut::setMin(double min)
{
  m_minIsSet = true;
  m_min = min;
}

void Cut::setMax(double max)
{
  m_maxIsSet = true;
  m_max = max;
}

bool Cut::passesCuts(double value) const
{
  if (m_minIsSet && m_maxIsSet)
    return (m_min <= value && value <= m_max);
  else if (m_minIsSet)
    return (m_min <= value);
  else if (m_maxIsSet)
    return (value <= m_max);
  else
    return true;
}

bool Cut::passes(const SimpleEvent* event) const
{
  if (m_type == Enums::CherenkovCut) {
    //get settings if present
    const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
    if (!settings || settings->situation() != Enums::Testbeam11)
      return true;
    double c1Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV1);
    double c2Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV2);
    return (passesCuts(c1Signal) && passesCuts(c2Signal));
  }
  return true;
}

bool Cut::passes(const QVector<Hit*>& clusters, const Particle* particle) const
{
  if (m_type == Enums::RigidityCut) {
    const Track* track = particle->track();
    if (!track || !track->fitGood())
      return false;
    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
      return false;
    return passesCuts(track->rigidity());
  }
  if (m_type == Enums::BetaCut) {
    const Track* track = particle->track();
    if (!track || !track->fitGood())
      return false;
    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
      return false;
    return passesCuts(particle->beta());
  }
  if (m_type == Enums::TimeOverThresholdCut) {
    const Track* track = particle->track();
    if (!track || !track->fitGood())
      return false;
    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
      return false;
    double totSum = 0.;
    int nTofHits = 0;
    const QVector<Hit*>::const_iterator endIt = clusters.end();
    for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
      Hit* hit = *clusterIt;
      if (hit->type() == Hit::tof) {
        TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
        double z = tofCluster->position().z();
        if (qAbs(track->x(z) - tofCluster->position().x()) > 0.95 * Constants::tofBarWidth / 2.)
          continue;
        const std::vector<Hit*>& subHits = tofCluster->hits();
        std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
        for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
          TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(*it);
          totSum+= tofSipmHit->timeOverThreshold();
          ++nTofHits;
        }
      }
    }
    if (nTofHits > 0) {
      return passesCuts(totSum / nTofHits);
    }
    else {
      return false;
    }
  }
  if (m_type == Enums::TrdSignalCut) {
    const Track* track = particle->track();
    if (!track || !track->fitGood())
      return false;
    ParticleInformation::Flags flags = particle->information()->flags();
    if (!(flags & (ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet)))
      return false;
    return passesCuts(sumOfSignalHeights(Hit::trd, track, clusters));
  }
  return true;
}

double Cut::sumOfSignalHeights(const Hit::ModuleType type, const Track* track, const QVector<Hit*>& /*clusters*/) const
{
  double sumSignal = 0;
  const QVector<Hit*>::const_iterator endIt = track->hits().constEnd();
  for (QVector<Hit*>::const_iterator clusterIt = track->hits().constBegin(); clusterIt != endIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() == type) {
      sumSignal += hit->signalHeight();
    }
  }
  return sumSignal;
}

QString Cut::toString() const
{
  QString string = Enums::label(m_type) + " | ";
  if (m_minIsSet)
    string+= QString::number(m_min);
  string+= " | ";
  if (m_maxIsSet)
    string+= QString::number(m_max);
  return string;
}
