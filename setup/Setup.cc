#include "Setup.hh"
#include "Helpers.hh"

#include "Cluster.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Layer.hh"
#include "DetectorElement.hh"
#include "SipmArray.hh"
#include "TRDModule.hh"
#include "TOFBar.hh"
#include "Enums.hh"

#include "InhomField.hh"
#include "UniformField.hh"

#include <QStringList>
#include <QSettings>
#include <QMutex>
#include <QDebug>
#include <QDir>

#include <TVector3.h>

#include <iostream>
#include <cmath>
#include <QDebug>

Setup* Setup::s_instance = 0;
QMutex Setup::s_mutex;

Setup::Setup() :
  m_coordinates(0),
  m_settings(0),
  m_field(0)
{
  QString path = Helpers::analysisPath() + "/conf/";
  QDir dir(path);
  if (!dir.exists("coordinates.conf")) {
    qFatal("ERROR: coordinates.conf not found. Maybe you need to switch to a configuration, for example: switch_to_config.sh kiruna");
  }
  if (!dir.exists("setup.conf")) {
    qFatal("ERROR: setup.conf not found. Maybe you need to switch to a configuration, for example: switch_to_config.sh kiruna");
  }

  m_coordinates = new QSettings(path + "coordinates.conf", QSettings::IniFormat);
  m_settings = new QSettings(path + "setup.conf", QSettings::IniFormat);

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

  delete m_field;
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
        m_field = new UniformField(TVector3(Bx, By, Bz));
      }
      else if (type == "inhom") {
        QString fieldMap = m_settings->value(key).toString();
        m_field = new InhomField(qPrintable(fieldMap));
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

bool Setup::isTrackerId(unsigned short id)
{
  return ((0x3000 <= id && id < 0x3200) || (0x3300 <= id && id < 0x3400) || (0x3700 <= id && id < 0x3800) || (0x6000 <= id && id < 0x8000));
}

bool Setup::isTofId(unsigned short id)
{
  return (0x8000 <= id && id <= 0x803f);
}

bool Setup::isTrdId(unsigned short id)
{
  return ((0x3200 <= id && id < 0x3300) || (0x3400 <= id && id < 0x3700));
}

SensorTypes::Type Setup::sensorForId(unsigned short id)
{
  if (isTrackerId(id))
    return trackerSensorForId(id);
  if (isTofId(id))
    return tofSensorForId(id);
  if (isTrdId(id))
    Q_ASSERT(false); //TODO: TRD
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

SensorTypes::Type Setup::trackerSensorForId(unsigned short id)
{
  if (0x3000 <= id && id <= 0x30ff) return (id - 0x3000 < 128) ? SensorTypes::HPE_0x3000_TEMP : SensorTypes::HPE_0x3001_TEMP;
  if (0x3100 <= id && id <= 0x31ff) return (id - 0x3100 < 128) ? SensorTypes::HPE_0x3100_TEMP : SensorTypes::HPE_0x3101_TEMP;
  if (0x3300 <= id && id <= 0x33ff) return (id - 0x3300 < 128) ? SensorTypes::HPE_0x3300_TEMP : SensorTypes::HPE_0x3301_TEMP;
  if (0x3700 <= id && id <= 0x37ff) return (id - 0x3700 < 128) ? SensorTypes::HPE_0x3700_TEMP : SensorTypes::HPE_0x3701_TEMP;
  if (0x6000 <= id && id <= 0x60ff) return (id - 0x6000 < 128) ? SensorTypes::HPE_0x6000_TEMP : SensorTypes::HPE_0x6001_TEMP;
  if (0x6100 <= id && id <= 0x61ff) return (id - 0x6100 < 128) ? SensorTypes::HPE_0x6100_TEMP : SensorTypes::HPE_0x6101_TEMP;
  if (0x6200 <= id && id <= 0x62ff) return (id - 0x6200 < 128) ? SensorTypes::HPE_0x6200_TEMP : SensorTypes::HPE_0x6201_TEMP;
  if (0x6300 <= id && id <= 0x63ff) return (id - 0x6300 < 128) ? SensorTypes::HPE_0x6300_TEMP : SensorTypes::HPE_0x6301_TEMP;
  if (0x6400 <= id && id <= 0x64ff) return (id - 0x6400 < 128) ? SensorTypes::HPE_0x6400_TEMP : SensorTypes::HPE_0x6401_TEMP;
  if (0x6500 <= id && id <= 0x65ff) return (id - 0x6500 < 128) ? SensorTypes::HPE_0x6500_TEMP : SensorTypes::HPE_0x6501_TEMP;
  if (0x6600 <= id && id <= 0x66ff) return (id - 0x6600 < 128) ? SensorTypes::HPE_0x6600_TEMP : SensorTypes::HPE_0x6601_TEMP;
  if (0x6700 <= id && id <= 0x67ff) return (id - 0x6700 < 128) ? SensorTypes::HPE_0x6700_TEMP : SensorTypes::HPE_0x6701_TEMP;
  if (0x7800 <= id && id <= 0x78ff) return (id - 0x7800 < 128) ? SensorTypes::HPE_0x7800_TEMP : SensorTypes::HPE_0x7801_TEMP;
  if (0x7900 <= id && id <= 0x79ff) return (id - 0x7900 < 128) ? SensorTypes::HPE_0x7900_TEMP : SensorTypes::HPE_0x7901_TEMP;
  if (0x7a00 <= id && id <= 0x7aff) return (id - 0x7a00 < 128) ? SensorTypes::HPE_0x7a00_TEMP : SensorTypes::HPE_0x7a01_TEMP;
  if (0x7b00 <= id && id <= 0x7bff) return (id - 0x7b00 < 128) ? SensorTypes::HPE_0x7b00_TEMP : SensorTypes::HPE_0x7b01_TEMP;
  if (0x7c00 <= id && id <= 0x7cff) return (id - 0x7c00 < 128) ? SensorTypes::HPE_0x7c00_TEMP : SensorTypes::HPE_0x7c01_TEMP;
  if (0x7d00 <= id && id <= 0x7dff) return (id - 0x7d00 < 128) ? SensorTypes::HPE_0x7d00_TEMP : SensorTypes::HPE_0x7d01_TEMP;
  if (0x7e00 <= id && id <= 0x7eff) return (id - 0x7e00 < 128) ? SensorTypes::HPE_0x7e00_TEMP : SensorTypes::HPE_0x7e01_TEMP;
  if (0x7f00 <= id && id <= 0x7fff) return (id - 0x7f00 < 128) ? SensorTypes::HPE_0x7f00_TEMP : SensorTypes::HPE_0x7f01_TEMP;
  Q_ASSERT(false);
  return SensorTypes::N_SENSOR_TYPES;
}
