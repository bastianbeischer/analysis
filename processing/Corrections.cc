#include "Corrections.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOFBar.hh"
#include "Particle.hh"
#include "Constants.hh"
#include "SensorTypes.hh"
#include "SimpleEvent.hh"

#include <QStringList>
#include <QSettings>
#include <QDebug>

#include <cstring>
#include <cmath>

Corrections::Corrections(Flags flags)
  : m_tofTotScalingPrefix("TimeOverThresholdScaling/")
  , m_trdSettings(0)
  , m_tofSettings(0)
  , m_flags(flags)
{
  char* env = getenv("PERDAIXANA_PATH");
  if (env == 0) {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  QString path(env);
  path += "/conf/";
  m_trdSettings = new QSettings(path + "TRDCorrections.conf", QSettings::IniFormat);
  m_tofSettings = new QSettings(path + "TOFCorrections.conf", QSettings::IniFormat);
  
  loadTotScaling();
}

Corrections::~Corrections()
{
  delete m_trdSettings;
  delete m_tofSettings;
}

void Corrections::preFitCorrections(SimpleEvent* event)
{
  const std::vector<Hit*>::const_iterator hitsEnd = event->hits().end();
  for (std::vector<Hit*>::const_iterator it = event->hits().begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    if (m_flags & Alignment) alignment(hit);
    if (m_flags & TimeShifts) timeShift(hit);
    if (m_flags & TrdMopv) trdMopv(hit);
    if (m_flags & TofTimeOverThreshold) {
      tofTot(hit, event);
    }
  }
}

void Corrections::postFitCorrections(Particle* particle)
{
  for (int i = 0; i < 5; i++) {
    if (m_flags & PhotonTravelTime) photonTravelTime(particle); // multiple scattering needs "beta"
    if (m_flags & MultipleScattering) multipleScattering(particle); // should be done first
  }
}

void Corrections::alignment(Hit* hit)
{
  Setup* setup = Setup::instance();
  DetectorElement* element = setup->element(hit->detId() - hit->channel());
  if (element->alignmentShift() != 0.)
    hit->setPosition(element->positionForHit(hit));
}

void Corrections::timeShift(Hit* hit)
{
  Setup* setup = Setup::instance();
  if (strcmp(hit->ClassName(), "TOFSipmHit") == 0) {
    TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
    DetectorElement* element = setup->element(hit->detId() - hit->channel());
    double timeShift = static_cast<TOFBar*>(element)->timeShift(hit->channel());
    tofHit->applyTimeShift(timeShift);
  }
  else if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
    TOFCluster* cluster = static_cast<TOFCluster*>(hit);
    std::vector<Hit*> subHits = cluster->hits();
    for (std::vector<Hit*>::iterator it = subHits.begin(); it != subHits.end(); it++) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(*it);
      DetectorElement* element = setup->element(cluster->detId() - cluster->channel());
      double timeShift = static_cast<TOFBar*>(element)->timeShift((*it)->channel());
      
      unsigned short id = element->id();
      double barTimeShift = 0;
      if (id == 0x8000 || id == 0x8010) {
        barTimeShift = 0;
      } else if (id == 0x8004 || id == 0x8014) {
        barTimeShift = -.424683288999999964;
      } else if (id == 0x8008 || id == 0x8018) {
        barTimeShift = -.154683288999999974;
      } else if (id == 0x800c || id == 0x801c) {
        barTimeShift = -0.324999999999999872e-1;
      } else if (id == 0x8020 || id == 0x8030) {
        barTimeShift = -.120693492999999999;
      } else if (id == 0x8024 || id == 0x8034) {
        barTimeShift = 0.303989795999999979;
      } else if (id == 0x8028 || id == 0x8038) {
        barTimeShift = -.343510203999999986;
      } else if (id == 0x802c || id == 0x803c) {
        barTimeShift = 0.269306507000000028;
      }
      tofHit->applyTimeShift(timeShift + barTimeShift);
    }
    cluster->processHits();
  }
}

void Corrections::trdMopv(Hit* hit)
{
  //only process TRD hits
  if ( hit->type() != Hit::trd )
    return;

  if (strcmp(hit->ClassName(), "Hit") == 0) {
    double trdScalingFactor = this->trdScalingFactor(hit->detId());
    hit->setSignalHeight(hit->signalHeight() * trdScalingFactor);
  }
  else if (strcmp(hit->ClassName(), "Cluster") == 0) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    double clusterAmplitude = 0;
    for (std::vector<Hit*>::iterator it = cluster->hits().begin(); it != cluster->hits().end(); it++) {
      Hit* trdHit = *it;
      double trdScalingFactor = this->trdScalingFactor(trdHit->detId());
      double newHitAmplitude = trdHit->signalHeight() * trdScalingFactor;
      trdHit->setSignalHeight(newHitAmplitude);
      clusterAmplitude += newHitAmplitude;
    }
    cluster->setSignalHeight(clusterAmplitude);
  }
}

void Corrections::tofTot(Hit* hit, SimpleEvent* event)
{
  if (hit->type() == Hit::tof) {
    TOFCluster* cluster = static_cast<TOFCluster*> (hit);
    std::vector<Hit*>::const_iterator it = cluster->hits().begin();
    std::vector<Hit*>::const_iterator itEnd = cluster->hits().end();
    for (; it != itEnd; ++it) {
      double temperature = event->sensorData(Setup::instance()->sensorForId((*it)->detId()));
      double scalingFactor = totScalingFactor((*it)->detId(), temperature);
      TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(*it);
      tofSipmHit->setTimeOverThreshold(tofSipmHit->timeOverThreshold() * scalingFactor);
    }
  }
}

double Corrections::trdScalingFactor(unsigned int channel)
{
  return m_trdSettings->value("ConstScaleFactor/" + QString::number(channel, 16), 1.).toDouble();
}

void Corrections::setTrdScalingFactor(unsigned int channel, double value)
{
  m_trdSettings->setValue("ConstScaleFactor/" + QString::number(channel, 16), value);
  m_trdSettings->sync();
}

double Corrections::photonTravelTime(double bending, double nonBending, double* p)
{
  double a = nonBending * 2. / Constants::tofBarLength;
  double b = bending * 2. / Constants::tofBarWidth;
  Q_ASSERT(qAbs(a) <= 1.);
  Q_ASSERT(qAbs(b) <= 1.);
  if (a > 0) return p[0] * a;
  double c = (b < 0) ? p[1] : p[2];
  return p[0] * a + c * pow(qAbs(a), 8) * (1-cos(M_PI * b));
}

double Corrections::photonTravelTimeDifference(double bending, double nonBending, double* p)
{
  double p1[nPhotonTravelTimeParameters];
  double p2[nPhotonTravelTimeParameters];
  p1[0] = p2[0] = p[1];
  for (int i = 1; i < nPhotonTravelTimeParameters; ++i) {
    p1[i] = p[i+1];
    p2[i] = p[i+nPhotonTravelTimeParameters];
  }
  return p[0] + photonTravelTime(bending, nonBending, p1) - photonTravelTime(bending, -nonBending, p2);
}

void Corrections::multipleScattering(Particle* particle)
{
  Track* track = particle->track();

  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tracker) {
      double p = track->rigidity();
      double sipmRes = hit->resolutionEstimate();
      double beta = particle->beta();
      // double m = particle->mass();
      // double beta = p / sqrt(p*p + m*m);
      double z = hit->position().z();
      z = round(z);
      double parameter = 0;
      if (qAbs(z) == 236) parameter = 11e-3;
      else if (qAbs(z) == 218) parameter = 7.4e-3;
      else if (qAbs(z) == 69) parameter = 35e-3;
      else if (qAbs(z) == 51) parameter = 31e-3;

      double mscPart = 1/(p*beta) * parameter;
      double newRes = sqrt(sipmRes*sipmRes + mscPart*mscPart);
      hit->setResolution(newRes);
    }
  }
  track->fit(track->hits());
}

void Corrections::photonTravelTime(Particle* particle)
{
  Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* cluster = *it;
    if (!strcmp(cluster->ClassName(), "TOFCluster")) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(cluster);
      int id = tofCluster->detId();
      double p[nPhotonTravelTimeDifferenceParameters];
      QList<QVariant> plist = m_tofSettings->value(QString("PhotonTravelTimeConstants/%1").arg(id, 0, 16)).toList();
      for (int i = 0; i < nPhotonTravelTimeDifferenceParameters; ++i) {
        p[i] = plist[i].toDouble();
      }
      double p1[nPhotonTravelTimeParameters];
      double p2[nPhotonTravelTimeParameters];
      p1[0] = p2[0] = p[1];
      for (int i = 1; i < nPhotonTravelTimeParameters; ++i) {
        p1[i] = p[i+1];
        p2[i] = p[i+nPhotonTravelTimeParameters];
      }
      for (unsigned int i = 0; i < tofCluster->hits().size(); ++i) {
        TOFSipmHit* hit = static_cast<TOFSipmHit*>(tofCluster->hits()[i]);
        double bending = (track->x(cluster->position().z()) - cluster->position().x());
        double nonBending = track->y(cluster->position().z());
        if (qAbs(bending) <= Constants::tofBarWidth / 2. && qAbs(nonBending) <= Constants::tofBarLength / 2.) {
          if (hit->position().y() < 0) {
            hit->setPhotonTravelTime(photonTravelTime(bending, nonBending, p1) + p[0]);
          } else {
            hit->setPhotonTravelTime(photonTravelTime(bending, -nonBending, p2));
          }
        }
      }
    }
  }
}

void Corrections::setTotScaling(const unsigned int tofId, const QList<QVariant> parameter)
{
  m_tofSettings->setValue(m_tofTotScalingPrefix + QString::number(tofId, 16), parameter);
  m_tofSettings->sync();
}

void Corrections::loadTotScaling()
{
  QString prefix = m_tofTotScalingPrefix;
  prefix.remove("/");
  if (!m_tofSettings->childGroups().contains(prefix)) {
    const QString& fileName =  m_tofSettings->fileName();
    qDebug() << QString("ERROR: %1 does not contain any %2").arg(fileName, prefix);
    Q_ASSERT(false);
  }
  for (unsigned int tofId = 0x8000; tofId <= 0x803f; ++tofId) {
    QString key = QString(m_tofTotScalingPrefix + "%1").arg(tofId, 0, 16);
    QList<QVariant> parameters = m_tofSettings->value(key).toList();
    for (int parameter = 0; parameter < parameters.size(); ++parameter) {
      m_totScalings[tofChannel(tofId)][parameter] = parameters[parameter].toDouble();
    }
  }
}

double Corrections::totScalingFactor(const unsigned int tofId, const double temperature)
{
  double value = m_totScalings[tofChannel(tofId)][0] + m_totScalings[tofChannel(tofId)][1] * temperature;
  return (value == 0) ? 0 : Constants::idealTot / value;
  //TODO: some kind of information if the temperature value is valid for this scaling
}

unsigned int Corrections::tofChannel(unsigned int tofId)
{
  const unsigned int firstTofId = 0x8000;
  return tofId - firstTofId;
}
