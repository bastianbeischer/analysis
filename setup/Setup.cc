#include "Setup.hh"

#include "Cluster.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Layer.hh"
#include "DetectorElement.hh"
#include "SipmArray.hh"
#include "TRDModule.hh"
#include "TOFBar.hh"

#include <QStringList>
#include <QSettings>
#include <QProcess>
#include <QMutex>

#include <TVector3.h>

#include <iostream>
#include <cmath>

Setup* Setup::s_instance = 0;
QMutex Setup::s_mutex;

Setup::Setup() :
  m_coordinates(0),
  m_settings(0)
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
  m_coordinates = new QSettings(path+"perdaix_coordinates.conf", QSettings::IniFormat);
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
      Layer* layer = this->layer(z);
      QStringList detIds = m_settings->value(key).toStringList();
      foreach(QString detId, detIds) {
        bool ok;
        unsigned short id = detId.toUShort(&ok, 16);
        if (ok) {
          DetectorElement* element = this->element(id);
          layer->addElement(element);
        }
      } // elements
      layer->sortIdsByPosition();
    }
  } // layers
}

Layer* Setup::layer(double z)
{
  // round to two digits.
  z = round(z*100.)/100.;

  if (!m_layers[z]) m_layers[z] = new Layer(z);
  return m_layers[z];
}

DetectorElement* Setup::element(unsigned short id)
{
  unsigned short usbBoard = (id >> 8) << 8;
  
  if (!m_elements[id]) {
    if (usbBoard == 0x3200 || usbBoard == 0x3600 || usbBoard == 0x3400 || usbBoard == 0x3500)
      m_elements[id] = new TRDModule(id, this);
    else if (usbBoard == 0x8000)
      m_elements[id] = new TOFBar(id, this);
    else
      m_elements[id] = new SipmArray(id, this);
  }

  return m_elements[id];
}

QVector<Hit*> Setup::generateClusters(const QVector<Hit*>& hits)
{
  QVector<Hit*> clusters;

  addHitsToLayers(hits);
  const LayerIterator endIt = lastLayer();
  for (LayerIterator it = firstLayer(); it != endIt; ++it) {
    Layer* layer = *it;
    clusters += layer->clusters();

    // // alternative: use only the best cluster
    // Cluster* cluster = layer->bestCluster();
    // if (cluster)
    //   clusters.push_back(cluster);

    // update pointer
  }

  return clusters;
}

void Setup::addHitsToLayers(const QVector<Hit*>& hits)
{
  // remove old hits
  clearHitsFromLayers();

  foreach(Hit* hit, hits) {
    double z = hit->position().z();
    layer(z)->addHitToDetector(hit);
  }
}

void Setup::clearHitsFromLayers()
{
  foreach(Layer* layer, m_layers)
    layer->clearHitsInDetectors();
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
