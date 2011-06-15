#include "Setup.hh"

#include "Cluster.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Layer.hh"
#include "DetectorElement.hh"
#include "SipmArray.hh"
#include "TRDModule.hh"
#include "TOFBar.hh"

#include "FieldManager.hh"
#include "InhomField.hh"
#include "UniformField.hh"

#include <QStringList>
#include <QSettings>
#include <QMutex>
#include <QDir>

#include <TVector3.h>

#include <iostream>
#include <cmath>

Setup* Setup::s_instance = 0;
QMutex Setup::s_mutex;

Setup::Setup() :
  m_coordinates(0),
  m_settings(0)
{
  const char* env = getenv("PERDAIXANA_PATH");
  if (env == 0) {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  QString path(env);
  path += "/conf/";
  QDir dir(path);
  if (!dir.exists("coordinates.conf")) {
    qFatal("ERROR: coordinates.conf not found. Maybe you need to switch to a configuration, for example: switch_to_config.sh kiruna");
  }
  if (!dir.exists("setup.conf")) {
    qFatal("ERROR: setup.conf not found. Maybe you need to switch to a configuration, for example: switch_to_config.sh kiruna");
  }

  m_coordinates = new QSettings(path+"coordinates.conf", QSettings::IniFormat);
  m_settings = new QSettings(path+"setup.conf", QSettings::IniFormat);

  construct();
}

Setup::~Setup()
{
  delete m_coordinates;
  delete m_settings;

  foreach(Layer* layer, m_layers)
    delete layer;
  foreach(DetectorElement* element, m_elements)
    delete element;
}

Setup* Setup::instance()
{
  Setup::s_mutex.lock();
  if (!s_instance) s_instance = new Setup;
  Setup::s_mutex.unlock();
  return s_instance;
}

void Setup::construct()
{
  Q_ASSERT(m_settings);
  foreach(QString key, m_settings->allKeys()) {
    QStringList list = key.split("/");
    if (list[0] == "layer") {
      double z = list[1].toDouble();
      Layer* layer = constructLayer(z);
      QStringList detIds = m_settings->value(key).toStringList();
      foreach(QString detId, detIds) {
        bool ok;
        unsigned short id = detId.toUShort(&ok, 16);
        if (ok) {
          DetectorElement* element = constructElement(id);
          layer->addElement(element);
        }
      } // elements
      layer->sortIdsByPosition();
    }
    else if (list[0] == "field") {
      QString type = list[1];
      if (type == "uniform") {
        QList<QVariant> values = m_settings->value(key).toList();
        double Bx = values[0].toDouble();
        double By = values[1].toDouble();
        double Bz = values[2].toDouble();
        FieldManager::instance()->setField(new UniformField(TVector3(Bx, By, Bz)));
      }
      else if (type == "inhom") {
        QString fieldMap = m_settings->value(key).toString();
        FieldManager::instance()->setField(new InhomField(qPrintable(fieldMap)));
      }
      else {
        // shouldn't happen
        Q_ASSERT(false);
      }
    }
  } // layers
}

Layer* Setup::constructLayer(double z)
{
  // round to two digits.
  z = round(z*100.)/100.;

  // should not exist already
  Q_ASSERT(!m_layers[z]);

  Layer* layer = new Layer(z);
  m_layers[z] = layer;
  return layer;
}

DetectorElement* Setup::constructElement(unsigned short id)
{
  unsigned short usbBoard = (id >> 8) << 8;
  
  // should not exist already
  Q_ASSERT(!m_elements[id]);

  DetectorElement* element = 0;
  if (usbBoard == 0x3200 || usbBoard == 0x3600 || usbBoard == 0x3400 || usbBoard == 0x3500)
    element = new TRDModule(id, this);
  else if (usbBoard == 0x8000)
    element = new TOFBar(id, this);
  else
    element = new SipmArray(id, this);

  m_elements[id] = element;
  return element;
}

Layer* Setup::layer(double z)
{
  // round to two digits.
  z = round(z*100.)/100.;
  Layer* layer = m_layers[z];
  Q_ASSERT(layer);
  return layer;
}

DetectorElement* Setup::element(unsigned short id)
{
  DetectorElement* element = m_elements[id];
  Q_ASSERT(element);
  return element;
}

TVector3 Setup::configFilePosition(QString group, unsigned short detId) const
{
  Q_ASSERT(m_coordinates);
  QList<QVariant> list = m_coordinates->value(group+"/"+QString::number(detId,16)).toList();
  return TVector3(list[0].toDouble(), list[1].toDouble(), list[2].toDouble());
}

double Setup::configFileAlignmentShift(QString group, unsigned short detId) const
{
  Q_ASSERT(m_settings);
  return m_settings->value(group+"/0x"+QString::number(detId,16)).toDouble();
}

double Setup::configFileTimeShift(unsigned short detId) const
{
  Q_ASSERT(m_settings);
  return m_settings->value("tofTimeShift/0x"+QString::number(detId,16), 1).toDouble();
}

void Setup::writeSettings()
{
  Q_ASSERT(m_settings);
  // layer section
  m_settings->beginGroup("layer");
  foreach(Layer* layer, m_layers) {
    double z = layer->z();
    QList<unsigned short> detIds = layer->detIds();
    QStringList stringList;
    foreach(unsigned short detId, detIds)
      stringList.push_back(QString("0x%1").arg(detId,0,16));
    QString key = QString("%1").arg(z);

    m_settings->setValue(key, QVariant(stringList));
  }
  m_settings->endGroup();

  // tracker, trd and tof sections
  foreach(DetectorElement* element, m_elements) {
    QString typeString;
    unsigned short type = element->type();
    if (type == DetectorElement::tracker) typeString = "tracker";
    if (type == DetectorElement::trd)     typeString = "trd";
    if (type == DetectorElement::tof)     typeString = "tof";

    m_settings->setValue(typeString + "/" + QString("0x%1").arg(element->id(), 0, 16), element->alignmentShift());

    if (type == DetectorElement::tof) {
      for (unsigned short channel = 0; channel < element->nChannels(); channel++) {
        unsigned short channelID = element->id() | channel;
        double timeShift = static_cast<TOFBar*>(element)->timeShift(channel);
        m_settings->setValue(typeString + "tofTimeShift/" + QString("0x%1").arg(channelID, 0, 16), timeShift);
      }
    }
  }

  // in order for the file to end up on disk we need to call "sync" here
  m_settings->sync();
}

SensorTypes::Type Setup::sensorForId(unsigned short id)
{
  if (0x8000 <= id && id <= 0x803f)
    return tofSensorForId(id);
  //TODO: Tracker, TRD
  Q_ASSERT(false);
  return SensorTypes::N_SENSOR_TYPES;
}

SensorTypes::Type Setup::tofSensorForId(unsigned short id)
{
  int channel = id - 0x8000;
  static const SensorTypes::Type map[] = {
    SensorTypes::TOF_2_TEMP, SensorTypes::TOF_2_TEMP, SensorTypes::TOF_4_TEMP, SensorTypes::TOF_4_TEMP,
    SensorTypes::TOF_2_TEMP, SensorTypes::TOF_2_TEMP, SensorTypes::TOF_4_TEMP, SensorTypes::TOF_4_TEMP,
    SensorTypes::TOF_1_TEMP, SensorTypes::TOF_1_TEMP, SensorTypes::TOF_3_TEMP, SensorTypes::TOF_3_TEMP,
    SensorTypes::TOF_1_TEMP, SensorTypes::TOF_1_TEMP, SensorTypes::TOF_3_TEMP, SensorTypes::TOF_3_TEMP,
    
    SensorTypes::TOF_2_TEMP, SensorTypes::TOF_2_TEMP, SensorTypes::TOF_4_TEMP, SensorTypes::TOF_4_TEMP,
    SensorTypes::TOF_2_TEMP, SensorTypes::TOF_2_TEMP, SensorTypes::TOF_4_TEMP, SensorTypes::TOF_4_TEMP,
    SensorTypes::TOF_1_TEMP, SensorTypes::TOF_1_TEMP, SensorTypes::TOF_3_TEMP, SensorTypes::TOF_3_TEMP,
    SensorTypes::TOF_1_TEMP, SensorTypes::TOF_1_TEMP, SensorTypes::TOF_3_TEMP, SensorTypes::TOF_3_TEMP,
    
    SensorTypes::TOF_6_TEMP, SensorTypes::TOF_6_TEMP, SensorTypes::TOF_8_TEMP, SensorTypes::TOF_8_TEMP,
    SensorTypes::TOF_6_TEMP, SensorTypes::TOF_6_TEMP, SensorTypes::TOF_8_TEMP, SensorTypes::TOF_8_TEMP,
    SensorTypes::TOF_5_TEMP, SensorTypes::TOF_5_TEMP, SensorTypes::TOF_7_TEMP, SensorTypes::TOF_7_TEMP,
    SensorTypes::TOF_5_TEMP, SensorTypes::TOF_5_TEMP, SensorTypes::TOF_7_TEMP, SensorTypes::TOF_7_TEMP,
    
    SensorTypes::TOF_6_TEMP, SensorTypes::TOF_6_TEMP, SensorTypes::TOF_8_TEMP, SensorTypes::TOF_8_TEMP,
    SensorTypes::TOF_6_TEMP, SensorTypes::TOF_6_TEMP, SensorTypes::TOF_8_TEMP, SensorTypes::TOF_8_TEMP,
    SensorTypes::TOF_5_TEMP, SensorTypes::TOF_5_TEMP, SensorTypes::TOF_7_TEMP, SensorTypes::TOF_7_TEMP,
    SensorTypes::TOF_5_TEMP, SensorTypes::TOF_5_TEMP, SensorTypes::TOF_7_TEMP, SensorTypes::TOF_7_TEMP
  };
  return map[channel];
}
