#include "Corrections.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOFBar.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "Constants.hh"
#include "SensorTypes.hh"
#include "SimpleEvent.hh"

#include <QProcess>
#include <QSettings>
#include <QDebug>

#include <cstring>
#include <cmath>

bool Corrections::timeOverThresholdScalingLoaded = false;
QMap<unsigned int, QList<double> > Corrections::timeOverThresholdScalings;

Corrections::Corrections(Flags flags)
  : m_tofTimeOverThresholdScalingPrefix("TimeOverThresholdScaling/")
  , m_trdSettings(0)
  , m_tofSettings(0)
  , m_flags(flags)
{
  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_PATH=*"));
  QString path = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    path = entry.split("=").at(1);
    path += "/conf/";
  }
  else {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  m_trdSettings = new QSettings(path + "TRDCorrections.conf", QSettings::IniFormat);
  m_tofSettings = new QSettings(path + "TOFCorrections.conf", QSettings::IniFormat);
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
      loadTimeOverThresholdScaling();
      tofTimeOverThreshold(hit, event);
    }
  }
}

void Corrections::postFitCorrections(Track* track)
{
  if (m_flags & PhotonTravelTime) photonTravelTime(track);
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
      tofHit->applyTimeShift(timeShift);
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

void Corrections::tofTimeOverThreshold(Hit* hit, SimpleEvent* event)
{
  if (hit->type() == Hit::tof) {
    TOFCluster* cluster = static_cast<TOFCluster*> (hit);
    foreach(Hit* tofHit, cluster->hits()) {
      double temperature = event->sensorData(Setup::instance()->sensorForId(tofHit->detId()));
      double scalingFactor = timeOverThresholdScalingFactor(tofHit->detId(), temperature);
      TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
      tofSipmHit->setTimeOverThreshold(tofSipmHit->timeOverThreshold() * scalingFactor);
    }
  }
}

double Corrections::trdScalingFactor(unsigned int channel)
{
  return m_trdSettings->value( "ConstScaleFactor/" + QString::number(channel,16) , 1.).toDouble();
}

void Corrections::setTrdScalingFactor(unsigned int channel, double value)
{
  m_trdSettings->setValue( "ConstScaleFactor/" + QString::number(channel,16), value);
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
  double p1[numberOfPhotonTravelTimeParameters];
  double p2[numberOfPhotonTravelTimeParameters];
  p1[0] = p2[0] = p[1];
  for (int i = 1; i < numberOfPhotonTravelTimeParameters; ++i) {
    p1[i] = p[i+1];
    p2[i] = p[i+numberOfPhotonTravelTimeParameters];
  }
  return p[0] + photonTravelTime(bending, nonBending, p1) - photonTravelTime(bending, -nonBending, p2);
}

void Corrections::photonTravelTime(Track* track)
{
  if (!track || !track->fitGood())
    return;

  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* cluster = *it;
    if (!strcmp(cluster->ClassName(), "TOFCluster")) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(cluster);
      int id = tofCluster->detId();
      double p[numberOfPhotonTravelTimeDifferenceParameters];
      QList<QVariant> plist = m_tofSettings->value(QString("PhotonTravelTimeConstants/%1").arg(id, 0, 16)).toList();
      for (int i = 0; i < numberOfPhotonTravelTimeDifferenceParameters; ++i) {
        p[i] = plist[i].toDouble();
        //qDebug() << p[i];
      }
      double p1[numberOfPhotonTravelTimeParameters];
      double p2[numberOfPhotonTravelTimeParameters];
      p1[0] = p2[0] = p[1];
      for (int i = 1; i < numberOfPhotonTravelTimeParameters; ++i) {
        p1[i] = p[i+1];
        p2[i] = p[i+numberOfPhotonTravelTimeParameters];
        //qDebug() << i << p1[i] << p2[i];
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

void Corrections::setTimeOverThresholdScaling(const unsigned int tofChannel, const QList<QVariant> param)
{
    m_tofSettings->setValue(m_tofTimeOverThresholdScalingPrefix+QString::number(tofChannel,16), param);
    m_tofSettings->sync();
}

void Corrections::loadTimeOverThresholdScaling()
{
  if (!Corrections::timeOverThresholdScalingLoaded) {
    QString prefix = m_tofTimeOverThresholdScalingPrefix;
    prefix.remove("/");
    if ( !m_tofSettings->childGroups().contains(prefix) ) {
      const QString fileName =  m_tofSettings->fileName();
      qDebug() << QString("ERROR: %1 does not contain any %2").arg(fileName, prefix);
    }
    for (unsigned int tofId = 0x8000; tofId <= 0x803f; tofId++) {
      QList<QVariant> params= m_tofSettings->value(QString(m_tofTimeOverThresholdScalingPrefix+"%1").arg(tofId,0,16)).toList();
      QList<double> paramsOutOfDoubles;
      for (int i = 0; i < params.size(); i++) {
        paramsOutOfDoubles.push_back(params[i].toDouble());
      }
      Corrections::timeOverThresholdScalings.insert(tofId, paramsOutOfDoubles);
    }
      
  }
  Corrections::timeOverThresholdScalingLoaded = true;
}

double Corrections::timeOverThresholdScalingFactor(const unsigned int tofChannel, const double temperature)
{
  const double timeOverThresholdReference = 30.;
  if (Corrections::timeOverThresholdScalings[tofChannel][0] == 0 && Corrections::timeOverThresholdScalings[tofChannel][1] == 0) {
    return 0;
  }
  else {
    return timeOverThresholdReference / (Corrections::timeOverThresholdScalings[tofChannel][0] + Corrections::timeOverThresholdScalings[tofChannel][1] *temperature);
  }
  //TODO: some kind of information if the temperature value is valid for this scaling
}
