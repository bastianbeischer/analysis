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
#include "TOFSensorTypeAssignment.hh"
#include "SimpleEvent.hh"

#include <QProcess>
#include <QSettings>
#include <QDebug>

#include <cstring>
#include <cmath>

Corrections::Corrections(Flags flags)
  : m_tofTimeOverThresholdPrefix("TimeOverThresholdScaling/")
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
    TOFSensorTypeAssignment* sensorAssignment = new TOFSensorTypeAssignment();
    TOFCluster* cluster = static_cast<TOFCluster*> (hit);
    foreach(Hit* tofHit, cluster->hits()) {
      double temperature = event->sensorData(sensorAssignment->tofSensorType(tofHit->detId()));
      double scalingFactor = timeOverThresholdScalingFactor(tofHit->detId(), temperature);
      TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
      tofSipmHit->setTimeOverThreshold(tofSipmHit->timeOverThreshold() * scalingFactor);
    }
    delete sensorAssignment;
    sensorAssignment = NULL;
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
      for (int i = 0; i < numberOfPhotonTravelTimeDifferenceParameters; ++i) {
        p[i] = m_tofSettings->value(QString("PhotonTravelTimeConstants/%1_c%2").arg(id, 0, 16).arg(i)).toDouble();
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

void Corrections::setTimeOverThresholdScaling(const unsigned int tofChannel, const QMap<QString, QVariant> temperatureMap)
{
    m_tofSettings->setValue(m_tofTimeOverThresholdPrefix+QString::number(tofChannel,16), temperatureMap);
    m_tofSettings->sync();
}

bool Corrections::totGraphsLoaded = false;
QMap<unsigned int, TGraph> Corrections::timeOverThresholdScalingGraphs;
QMap<unsigned int, TF1> Corrections::timeOverThresholdScalingFits;
QMap<unsigned int, double> Corrections::minTofTemps;
QMap<unsigned int , double> Corrections::maxTofTemps;

void Corrections::loadTimeOverThresholdScaling() 
{
  if ( !Corrections::totGraphsLoaded ) {
    
    QString prefix = m_tofTimeOverThresholdPrefix;
    prefix.remove("/");
    if ( !m_tofSettings->childGroups().contains(prefix) ) {
      const QString fileName =  m_tofSettings->fileName();
      qDebug() << QString("ERROR: %1 does not contain any %2").arg(fileName, prefix);
    }
    
    for (unsigned int tofChannel = 0x8000; tofChannel <= 0x803f; tofChannel++) {
      
      QMap<QString, QVariant> tempMap = m_tofSettings->value(QString(m_tofTimeOverThresholdPrefix+"%1").arg(tofChannel,0,16)).toMap();
      
      double min = tempMap.keys()[0].toDouble();
      double max = min;
      
      QMap<double, double> scalingMap;
      foreach(QString tempString, tempMap.keys()) {
        double temp = tempString.toDouble();
        double scalingFactor = tempMap[tempString].toDouble();
        scalingMap.insert(temp, scalingFactor);
        
        if (temp < min) {
          min = temp;
        }
        if (temp > max) {
          max = temp;
        }
      }
      
      double binSize;
      if (scalingMap.keys().size() > 1) {
        binSize = scalingMap.values()[1] - scalingMap.values()[0];
      }
      else {
        //if the map contains only one temperature value the binSize is arbitrarily set
        binSize = 2;
      }

      Corrections::minTofTemps.insert(tofChannel, min-binSize/2);
      Corrections::maxTofTemps.insert(tofChannel, max+binSize/2);
      
      
      TGraph graph;
      foreach(double temp, scalingMap.keys()) {
        int nPoints = graph.GetN();
        graph.SetPoint(nPoints, temp, scalingMap[temp]);
      }
      
      Corrections::timeOverThresholdScalingGraphs.insert(tofChannel, graph);
      
      //Linear fit
      QString htitle =QString("Fit time over threshold temperature dependent 0x%1").arg(tofChannel,0,16);
      TF1 f(qPrintable(htitle), "pol1");
      f.SetLineColor(kRed);
      f.SetLineWidth(1);
      f.SetLineStyle(2);
      graph.Fit(&f, "EQN0");
      Corrections::timeOverThresholdScalingFits.insert(tofChannel, f);
    }
    Corrections::totGraphsLoaded = true;
  }
  
}

double Corrections::timeOverThresholdScalingFactor(const unsigned int tofChannel, const double temperature)
{
  if (Corrections::minTofTemps[tofChannel] <= temperature && temperature <= Corrections::maxTofTemps[tofChannel]) {
    return Corrections::timeOverThresholdScalingFits[tofChannel].Eval(temperature);
  }
  //todo if temperature not in intervall
  else if(temperature > Corrections::maxTofTemps[tofChannel]) {
    qDebug() << QString("ERROR: temperature over scaling range, temperature = %1").arg(temperature);
    qDebug() << QString("0x%1").arg(tofChannel,0,16);
    qDebug() << Corrections::minTofTemps[tofChannel];
    qDebug() << Corrections::maxTofTemps[tofChannel];
    return 0;
  }
  else {
    qDebug() << QString("ERROR: temperature under scaling range, temperature = %1").arg(temperature);
    qDebug() << QString("0x%1").arg(tofChannel,0,16);
    qDebug() << Corrections::minTofTemps[tofChannel];
    qDebug() << Corrections::maxTofTemps[tofChannel];
    return 0;
  }
}
