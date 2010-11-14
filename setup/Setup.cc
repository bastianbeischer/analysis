#include "Setup.hh"

#include "Cluster.hh"
#include "Layer.hh"
#include "DetectorElement.hh"
#include "SipmArray.hh"
#include "TRDModule.hh"
#include "TOFBar.hh"

#include <QStringList>
#include <QSettings>
#include <QProcess>

#include <iostream>
#include <cmath>

Setup* Setup::m_instance = 0;

Setup::Setup() :
  m_settings(0),
  m_layerIt(0),
  m_elementIt(0)
{
  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_PATH=*"));
  QString path = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    path = entry.split("=").at(1);
    path += "/setup/";
  }
  m_settings = new QSettings(path+"setup.conf", QSettings::IniFormat);

  construct();
}

Setup::~Setup()
{
  delete m_settings;

  foreach(Layer* layer, m_layers)
    delete layer;
  foreach(DetectorElement* element, m_elements)
    delete element;
}

Setup* Setup::instance()
{
  if (!m_instance) m_instance = new Setup;
  return m_instance;
}

void Setup::construct()
{
  if (m_settings) {
    foreach(QString key, m_settings->allKeys()) {
      bool ok;
      QString group = key.split("/").at(0);

      // construct all layers and elements
      if (group == "layer") {
        double z = key.split("/").at(1).toDouble();
        Layer* layer = this->layer(z);

        QStringList detIds = m_settings->value(key).toStringList();
        foreach(QString detId, detIds) {
          unsigned short id = detId.toUShort(&ok, 16);
          if (ok) {
            DetectorElement* element = this->element(id);
            layer->addElement(element);
          }
        }
      } // construct layers and elements
      
      // set alignment parameters
      else {
        unsigned short detId = key.split("/").at(1).toUShort(&ok, 16);
        if (ok) {
          DetectorElement* element = this->element(detId);
          element->setAlignmentShift(m_settings->value(key).toDouble());
        }
      } // set alignment
    } // all keys
  }
}

Layer* Setup::firstLayer()
{
  if (m_layers.size() == 0) return 0;
  m_layerIt = m_layers.begin();
  return m_layerIt.value();
}

DetectorElement* Setup::firstElement()
{
  if (m_elements.size() == 0) return 0;
  m_elementIt = m_elements.begin();
  return m_elementIt.value();
}

Layer* Setup::nextLayer()
{
  ++m_layerIt;
  if (m_layerIt == m_layers.end())
    return 0;

  return m_layerIt.value();
}

DetectorElement* Setup::nextElement()
{
  ++m_elementIt;
  if (m_elementIt == m_elements.end())
    return 0;

  return m_elementIt.value();
}

Layer* Setup::layer(double z)
{
  // round to two digits.
  z *= 100.;
  z = round(z);
  z /= 100.;

  if (!m_layers[z]) m_layers[z] = new Layer(z);
  return m_layers[z];
}

DetectorElement* Setup::element(unsigned short id)
{
  // this should differentiate between types of detector elements according to the QSettings file soon
  unsigned short usbBoard = (id >> 8) << 8;
    
  if (!m_elements[id]) {
    if (usbBoard == 0x3200 || usbBoard == 0x3600 || usbBoard == 0x3400 || usbBoard == 0x3500)
      m_elements[id] = new TRDModule(id);
    else if (usbBoard == 0x8000) {
      m_elements[id] = new TOFBar(id);
    }
    else
      m_elements[id] = new SipmArray(id);
  }

  return m_elements[id];
}

QVector<Cluster*> Setup::generateClusters(QVector<Hit*> hits)
{
  QVector<Cluster*> clusters;

  clearClusters();
  bool needToFindClusters = false;
  foreach(Hit* hit, hits) {
    Cluster* cluster = dynamic_cast<Cluster*>(hit);
    if (cluster) {
      clusters.push_back(cluster);
    }
    else {
      needToFindClusters = true;
    }
  }

  if (needToFindClusters) {
    addHitsToLayers(hits);
    Layer* layer = firstLayer();
    while(layer) {
      clusters += layer->clusters();

      // // alternative: use only the best cluster
      // Cluster* cluster = layer->bestCluster();
      // if (cluster)
      //   clusters.push_back(cluster);

      // update pointer
      layer = nextLayer();
    }
  }
  return clusters;
}

void Setup::deleteClusters()
{
  foreach(DetectorElement* element, m_elements)
    element->deleteClusters();
}

void Setup::clearClusters()
{
  foreach(DetectorElement* element, m_elements)
    element->clearClusters();
}

void Setup::addHitsToLayers(QVector<Hit*> hits)
{
  // remove old hits
  clearHitsFromLayers();

  foreach(Hit* hit, hits) {
    double z = hit->position().z();
    layer(z)->addHitToDetector(hit);
  }
  foreach(Layer* layer, m_layers) {
    layer->sortHits();
  }
}

void Setup::clearHitsFromLayers()
{
  foreach(Layer* layer, m_layers)
    layer->clearHitsInDetectors();
}

void Setup::writeSettings()
{
  if (m_settings) {
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

      m_settings->setValue(typeString + "/" + QString("0x%1").arg(element->id(),0,16), element->alignmentShift());
    }

    // in order for the file to end up on disk we need to call "sync" here
    m_settings->sync();
  }
}

