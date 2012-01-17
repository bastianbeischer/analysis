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
#include "Helpers.hh"

#include <TSpline.h>
#include <TGraph.h>

#include <QStringList>
#include <QSettings>
#include <QMap>
#include <QDebug>
#include <QDir>

#include <cstring>
#include <cmath>

Corrections::Corrections(Enums::Corrections flags)
  : m_tofTotScalingPrefix("TimeOverThresholdScaling/")
  , m_trackerSignalScalingPrefix("TrackerSignalScaling/")
  , m_trdSettings(0)
  , m_trackerSettings(0)
  , m_tofSettings(0)
  , m_flags(flags)
  , m_TRDSplineTime(0)
  , m_timeKey("/time")
  , m_factorKey("/factor")
{
  QString path = Helpers::analysisPath() + "/conf/";
  QDir dir(path);
  if (!dir.exists("TRDCorrections.conf")) {
    qFatal("ERROR: TRDCorrections.conf not found. Maybe you need to switch to a configuration, for example: switch_to_config.sh kiruna");
  }
  if (!dir.exists("TOFCorrections.conf")) {
    qFatal("ERROR: TOFCorrections.conf not found. Maybe you need to switch to a configuration, for example: switch_to_config.sh kiruna");
  }
  if (!dir.exists("TrackerCorrections.conf")) {
    qFatal("ERROR: TrackerCorrections.conf not found. Maybe you need to switch to a configuration, for example: switch_to_config.sh kiruna");
  }
  m_trdSettings = new QSettings(path + "TRDCorrections.conf", QSettings::IniFormat);
  m_tofSettings = new QSettings(path + "TOFCorrections.conf", QSettings::IniFormat);
  m_trackerSettings = new QSettings(path + "TrackerCorrections.conf", QSettings::IniFormat);
  loadTotScaling();
  loadTrackerSignalScaling();
  readTRDTimeDependendCorrections();
}

Corrections::~Corrections()
{
  writeTRDTimeDependendCorrections();
  foreach (TSpline3* spline, m_trackerSignalSplines)
    delete spline;
  delete m_TRDSplineTime;
  delete m_trdSettings;
  delete m_tofSettings;
}

void Corrections::preFitCorrections(SimpleEvent* event)
{
  const std::vector<Hit*>::const_iterator hitsEnd = event->hits().end();
  for (std::vector<Hit*>::const_iterator it = event->hits().begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    if (m_flags & Enums::TrackerSignalHeight) trackerSignalHeight(hit, event);
    if (m_flags & Enums::Alignment) alignment(hit);
    if (m_flags & Enums::TimeShifts) timeShift(hit);
    if (m_flags & Enums::TrdMopv) trdMopv(hit);
    if (m_flags & Enums::TrdTime) trdTime(hit, event);
    //if (m_flags & Enums::TrdPressure) trdPressure(hit, event);
    //if (m_flags & Enums::TrdTemperature) trdTemperature(hit, event);
    if (m_flags & Enums::TofTimeOverThreshold) tofTot(hit, event);
  }
}

void Corrections::postFitCorrections(Particle* particle)
{
  if (m_flags & Enums::PhotonTravelTime) photonTravelTime(particle); // multiple scattering needs "beta"
}

void Corrections::postTOFCorrections(Particle* particle)
{
  if (m_flags & Enums::MultipleScattering) multipleScattering(particle);
}

void Corrections::alignment(Hit* hit)
{
  Setup* setup = Setup::instance();
  DetectorElement* element = setup->element(hit->elementId());
  if (element->alignmentShift() != 0.)
    hit->setPosition(element->positionForHit(hit));
}

void Corrections::timeShift(Hit* hit)
{
  Setup* setup = Setup::instance();
  if (strcmp(hit->ClassName(), "TOFSipmHit") == 0) {
    TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
    DetectorElement* element = setup->element(hit->elementId());
    double timeShift = static_cast<TOFBar*>(element)->timeShift(hit->channel());
    tofHit->applyTimeShift(timeShift);
  }
  else if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
    TOFCluster* cluster = static_cast<TOFCluster*>(hit);
    std::vector<Hit*> subHits = cluster->hits();
    for (std::vector<Hit*>::iterator it = subHits.begin(); it != subHits.end(); it++) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(*it);
      DetectorElement* element = setup->element(cluster->elementId());
      double timeShift = static_cast<TOFBar*>(element)->timeShift((*it)->channel());
      tofHit->applyTimeShift(timeShift);
    }
    cluster->processHits();
  }
}

void Corrections::trdMopv(Hit* hit)
{
  //only process TRD hits
  if (hit->type() != Hit::trd)
    return;

  if (strcmp(hit->ClassName(), "Hit") == 0) {
    double trdScalingFactor = this->trdScalingFactor(hit->detId());
    hit->setSignalHeight(hit->signalHeight() * trdScalingFactor);
  }
  else if (strcmp(hit->ClassName(), "Cluster") == 0) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    double clusterAmplitude = 0;
    for (std::vector<Hit*>::const_iterator it = cluster->hits().begin(); it != cluster->hits().end(); it++) {
      Hit* trdHit = *it;
      double trdScalingFactor = this->trdScalingFactor(trdHit->detId());
      double newHitAmplitude = trdHit->signalHeight() * trdScalingFactor;
      trdHit->setSignalHeight(newHitAmplitude);
      clusterAmplitude += newHitAmplitude;
    }
    cluster->setSignalHeight(clusterAmplitude);
  }
}

void Corrections::trdTime(Hit* hit, SimpleEvent* event)
{
  //only process TRD hits
  if (hit->type() != Hit::trd)
    return;

  double time = event->time();
  double trdTimeFactor = this->trdTimeDependendFactor(time);

  if (strcmp(hit->ClassName(), "Hit") == 0) {
    hit->setSignalHeight(hit->signalHeight() * trdTimeFactor);
  }
  else if (strcmp(hit->ClassName(), "Cluster") == 0) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    double clusterAmplitude = 0;
    for (std::vector<Hit*>::const_iterator it = cluster->hits().begin(); it != cluster->hits().end(); it++) {
      Hit* trdHit = *it;
      double newHitAmplitude = trdHit->signalHeight() * trdTimeFactor;
      trdHit->setSignalHeight(newHitAmplitude);
      clusterAmplitude += newHitAmplitude;
    }
    cluster->setSignalHeight(clusterAmplitude);
  }
}

void Corrections::trdPressure(Hit* hit, SimpleEvent* event)
{
  //only process TRD hits
  if (hit->type() != Hit::trd)
    return;

  double pressure = event->sensorData(SensorTypes::TRD_PRESSURE_SMOOTHED);
  double trdScalingFactor = this->trdPressureDependendFactor(pressure);

  if (strcmp(hit->ClassName(), "Hit") == 0) {
    hit->setSignalHeight(hit->signalHeight() * trdScalingFactor);
  }
  else if (strcmp(hit->ClassName(), "Cluster") == 0) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    double clusterAmplitude = 0;
    for (std::vector<Hit*>::const_iterator it = cluster->hits().begin(); it != cluster->hits().end(); it++) {
      Hit* trdHit = *it;
      double newHitAmplitude = trdHit->signalHeight() * trdScalingFactor;
      trdHit->setSignalHeight(newHitAmplitude);
      clusterAmplitude += newHitAmplitude;
    }
    cluster->setSignalHeight(clusterAmplitude);
  }
}

void Corrections::trdTemperature(Hit* hit, SimpleEvent* event)
{
  //only process TRD hits
  if (hit->type() != Hit::trd)
    return;

  double temperature = 0.;
  int count = 0;
  for (unsigned int i = SensorTypes::TRD_TUBE_TOP_HOT_TEMP; i <= SensorTypes::TRD_TUBE_BOTTOM_COLD_TEMP; i++) {
    temperature += event->sensorData((SensorTypes::Type)i);
    count++;
  }
  temperature /= count;

  double trdScalingFactor = this->trdTemperatureDependendFactor(temperature);

  if (strcmp(hit->ClassName(), "Hit") == 0) {
    hit->setSignalHeight(hit->signalHeight() * trdScalingFactor);
  }
  else if (strcmp(hit->ClassName(), "Cluster") == 0) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    double clusterAmplitude = 0;
    for (std::vector<Hit*>::const_iterator it = cluster->hits().begin(); it != cluster->hits().end(); it++) {
      Hit* trdHit = *it;
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

void Corrections::trackerSignalHeight(Hit* hit, SimpleEvent* event)
{
  if (hit->type() == Hit::tracker) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    const std::vector<Hit*>& subHits = cluster->hits();
    std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* subHit = *it;
      double scalingFactor = trackerSignalScalingFactor(subHit->elementId(), event->time());
      subHit->setSignalHeight(subHit->signalHeight() * scalingFactor);
    }
    cluster->processHits();
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

double Corrections::trdPressureDependendFactor(double P)
{
  double P0 = m_trdSettings->value("PressureDependency/P0", 1100).toDouble();
  double M0 = m_trdSettings->value("PressureDependency/M0", 1).toDouble();
  double dM_dP = m_trdSettings->value("PressureDependency/dM_dP", 0).toDouble();
  return M0 / ( M0 + (P-P0) * dM_dP);
}

void Corrections::setTrdPressureDependendFactor(QPair<double,double> P0, double dM_dP)
{
  m_trdSettings->setValue("PressureDependency/P0", P0.first);
  m_trdSettings->setValue("PressureDependency/M0", P0.second);
  m_trdSettings->setValue("PressureDependency/dM_dP", dM_dP);
  m_trdSettings->sync();
}

void Corrections::trdPressureDependendFactor(QPair<double,double>& P0, double& dM_dP)
{
  P0.first = m_trdSettings->value("PressureDependency/P0", 1100).toDouble();
  P0.second = m_trdSettings->value("PressureDependency/M0", 1).toDouble();
  dM_dP = m_trdSettings->value("PressureDependency/dM_dP", 0).toDouble();
}

double Corrections::trdTemperatureDependendFactor(double T)
{
  double T0 = m_trdSettings->value("TemperatureDependency/T0", 30).toDouble();
  double M0 = m_trdSettings->value("TemperatureDependency/M0", 1).toDouble();
  double dM_dT = m_trdSettings->value("TemperatureDependency/dM_dT", 0).toDouble();
  return M0 / ( M0 + (T-T0) * dM_dT);
}

void Corrections::setTrdTemperatureDependendFactor(QPair<double,double> T0, double dM_dT)
{
  m_trdSettings->setValue("TemperatureDependency/T0", T0.first);
  m_trdSettings->setValue("TemperatureDependency/M0", T0.second);
  m_trdSettings->setValue("TemperatureDependency/dM_dT", dM_dT);
  m_trdSettings->sync();
}

void Corrections::trdTemperatureDependendFactor(QPair<double,double>& T0, double& dM_dT)
{
  T0.first = m_trdSettings->value("TemperatureDependency/T0", 30).toDouble();
  T0.second = m_trdSettings->value("TemperatureDependency/M0", 1).toDouble();
  dM_dT = m_trdSettings->value("TemperatureDependency/dM_dT", 0).toDouble();
}

void Corrections::addTrdTimeDependendFactor(double time, double factor)
{
  m_TRDMapTime.insert(time, factor);
  //TODO needed?
  writeTRDTimeDependendCorrections();
}

void Corrections::removeTrdTimeDependendFactors(double startTime, double endTime)
{
  for (QMap<double, double>::const_iterator it = m_TRDMapTime.constBegin(); it != m_TRDMapTime.constEnd(); it++) {
    if (startTime <= it.key() && it.key() <= endTime) {
      qDebug("remove previous time correction factor at %f", it.key());
      m_TRDMapTime.remove(it.key());
    }
  }
}

double Corrections::trdTimeDependendFactor(double time)
{
  if (m_TRDSplineTime) {
    if (m_TRDSplineTime->GetXmin() <= time && time <= m_TRDSplineTime->GetXmax())
      return m_TRDSplineTime->Eval(time);
    else if (time < m_TRDSplineTime->GetXmin())
      return m_TRDSplineTime->Eval(m_TRDSplineTime->GetXmin());
    else if (time > m_TRDSplineTime->GetXmax())
      return m_TRDSplineTime->Eval(m_TRDSplineTime->GetXmax());
    else
      return 1;
  }
  else
    return 1;
}

void Corrections::writeTRDTimeDependendCorrections()
{
  QMap<double, double>::const_iterator it = m_TRDMapTime.constBegin();
  int i = 0;
  m_trdSettings->beginWriteArray("TimeDependendCorrection");
  for (it = m_TRDMapTime.constBegin(); it != m_TRDMapTime.constEnd(); it++) {
    m_trdSettings->setArrayIndex(i);
    m_trdSettings->setValue("time", it.key());
    m_trdSettings->setValue("factor", it.value());
    i++;
  }
  m_trdSettings->endArray();
  m_trdSettings->sync();
}

void Corrections::readTRDTimeDependendCorrections()
{
  int size = m_trdSettings->beginReadArray("TimeDependendCorrection");
  for (int i = 0; i < size; ++i) {
     m_trdSettings->setArrayIndex(i);
     double time = m_trdSettings->value("time").toDouble();
     double factor = m_trdSettings->value("factor").toDouble();
     m_TRDMapTime.insert(time, factor);
  }
  m_trdSettings->endArray();
  //create the interpolation spline
  if (m_TRDSplineTime) {
    delete m_TRDSplineTime;
    m_TRDSplineTime = 0;
  }
  if (m_TRDMapTime.size() > 2) {
    QVector<double> times(m_TRDMapTime.keys().toVector());
    QVector<double> factors(m_TRDMapTime.values().toVector());
    //insert points in parts ehere there is a calibration gap of t > maxDeltaT and insert linear interpolated points
    double maxDeltaT = 3600.*7.;
    bool insertedNewValue = true;
    while (insertedNewValue) {
      insertedNewValue = false;
      for (int i = 0; i < times.size()-1; ++i) {
        double t1 = times.at(i);
        double t2 = times.at(i+1);
        double f1 = factors.at(i);
        double f2 = factors.at(i+1);
        double deltaT = t2-t1;
        if (deltaT > maxDeltaT) {
          //qDebug("between spline points %i and %i is a time difference of %f > %f", i, i+1, deltaT, maxDeltaT);
          double m = (f2-f1)/deltaT;
          times.insert(i+1, t1+deltaT/2.);
          factors.insert(i+1, f1 + m*deltaT/2);
          insertedNewValue = true;
          break;
        }
      }
    }
    m_TRDSplineTime = new TSpline3("trdTimeSpline", &(*times.begin()), &(*factors.begin()), times.size(), "b1e1", 0., 0.);
  }
}

TSpline3* Corrections::trdTimeSpline() const
{
  if (!m_TRDSplineTime)
    return 0;
  else
    return new TSpline3(*m_TRDSplineTime);
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
  if (!track || !track->fitGood())
    return;

  for (int i = 0; i < 5; i++) {
    const QVector<Hit*>::ConstIterator hitsEnd = track->hits().end();
    for (QVector<Hit*>::ConstIterator it = track->hits().begin(); it != hitsEnd; ++it) {
      Hit* hit = *it;
      if (hit->type() == Hit::tracker) {
        double p = track->rigidity();
        double beta = particle->beta();
        if (beta == 0 || p == 0)
          return;
        // double m = particle->mass();
        // double beta = p / sqrt(p*p + m*m);
        double z = hit->position().z();
        z = round(z);
        double parameter = 0;
        if (qAbs(z) == 236) parameter = 11e-3;
        else if (qAbs(z) == 218) parameter = 7.4e-3;
        else if (qAbs(z) == 69) parameter = 35e-3;
        else if (qAbs(z) == 51) parameter = 31e-3;

        double sipmRes = hit->resolutionEstimate();
        double mscPart = 1/(p*beta) * parameter;
        double newRes = sqrt(sipmRes*sipmRes + mscPart*mscPart);
        hit->setResolution(newRes);
      }
    }
    track->fit(track->hits());
  }
}

void Corrections::photonTravelTime(Particle* particle)
{
  Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  const QVector<Hit*>::ConstIterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::ConstIterator it = track->hits().begin(); it != hitsEnd; ++it) {
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

void Corrections::writeTrackerSignalScaling(const unsigned short sipmId, const QVector<double>& times, const QVector<double>& factors)
{
  write(m_trackerSignalScalingPrefix + QString::number(sipmId, 16), m_trackerSettings, times, factors);
}

void Corrections::write(const QString& key, QSettings* settings, const QVector<double>& times, const QVector<double>& factors)
{
  QList<QVariant> timesVariant;
  QList<QVariant> factorsVariant;
  for (int i = 0; i < times.size(); ++i) {
    timesVariant.push_back(times[i]);
    factorsVariant.push_back(factors[i]);
  }
  settings->setValue(key + m_timeKey, timesVariant);
  settings->setValue(key + m_factorKey, factorsVariant);
  settings->sync();
}

TSpline3* Corrections::read(const QString& key, QSettings* settings)
{
  const QList<QVariant>& timesVariant = settings->value(key + m_timeKey).toList();
  const QList<QVariant>& factorsVariant = settings->value(key + m_factorKey).toList();
  Q_ASSERT(timesVariant.size() == factorsVariant.size());
  int n = timesVariant.size();
  QVector<double> times(n, 0);
  QVector<double> factors(n, 0);
  for (int i = 0; i < n; ++i) {
    times[i] = timesVariant[i].toDouble();
    factors[i] = factorsVariant[i].toDouble();
  }
  return new TSpline3(qPrintable(key + "_spline"), times.data(), factors.data(), n);
}

void Corrections::loadTrackerSignalScaling()
{
  QString prefix = m_trackerSignalScalingPrefix;
  prefix.remove("/");
  if (!m_trackerSettings->childGroups().contains(prefix)) {
    qDebug() << QString("ERROR: %1 does not contain any %2").arg(m_trackerSettings->fileName(), prefix);
    Q_ASSERT(false);
  }
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      unsigned short sipmId = element->id();
      const QString& key = QString(m_trackerSignalScalingPrefix + "%1").arg(sipmId, 0, 16);
      TSpline3* spline = read(key, m_trackerSettings);
      m_trackerSignalSplines.insert(sipmId, spline);
    }
  }
}

double Corrections::trackerSignalScalingFactor(const unsigned short sipmId, const double time)
{
  Q_ASSERT(m_trackerSignalSplines[sipmId]);
  TSpline3* spline = m_trackerSignalSplines[sipmId];
  Q_ASSERT(spline);
  if (spline->GetXmin() <= time && time <= spline->GetXmax())
    return spline->Eval(time);
  return 1.;
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
  double value = m_totScalings[tofChannel(tofId)][0];
  if (!std::isnan(temperature))
    value += m_totScalings[tofChannel(tofId)][1] * temperature;
  return (value == 0) ? 0 : Constants::idealTot / value;
  //TODO: some kind of information if the temperature value is valid for this scaling
}

unsigned int Corrections::tofChannel(unsigned int tofId)
{
  const unsigned int firstTofId = 0x8000;
  return tofId - firstTofId;
}
