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
#include <QDebug>

#include <TVector3.h>

#include <iostream>
#include <cassert>
#include <cmath>

Setup* Setup::m_instance = 0;

Setup::Setup() :
  m_coordinates(0),
  m_settings(0),
  m_layerIt(0),
  m_elementIt(0)
{
  m_instance = this; // this has to be set before construct()!

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
  if (!m_instance) new Setup;
  return m_instance;
}

void Setup::construct()
{
  if (m_settings) {
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
  } // settings
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
  z = round(z*100.)/100.;

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
    else if (usbBoard == 0x8000)
      m_elements[id] = new TOFBar(id);
    else
      m_elements[id] = new SipmArray(id);
  }

  return m_elements[id];
}

QVector<Hit*> Setup::generateClusters(const QVector<Hit*>& hits)
{
  QVector<Hit*> clusters;

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
  foreach(Layer* layer, m_layers) {
    layer->sortHits();
  }
}

void Setup::clearHitsFromLayers()
{
  foreach(Layer* layer, m_layers)
    layer->clearHitsInDetectors();
}

void Setup::applyCorrections(QVector<Hit*>& hits, CorrectionFlags flags)
{
  foreach(Hit* hit, hits) {
    if (flags & Alignment) {
      hit->setPosition(positionForHit(hit));
    }
    if (flags & TimeShifts) {
      if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
        TOFCluster* cluster = static_cast<TOFCluster*>(hit);
        std::vector<Hit*> subHits = cluster->hits();
        for (std::vector<Hit*>::iterator it = subHits.begin(); it != subHits.end(); it++) {
          TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(*it);
          double timeShift = timeShiftForHit(hit);
          tofHit->applyTimeShift(timeShift);
        }
        cluster->processHits();
      }
      else if (strcmp(hit->ClassName(), "TOFSipmHit") == 0) {
        TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
        double timeShift = timeShiftForHit(hit);
        tofHit->applyTimeShift(timeShift);
      }
    }
  }
}

TVector3 Setup::positionForHit(const Hit* hit)
{
  DetectorElement* element = this->element(hit->detId() - hit->channel());
  return element->positionForHit(hit);
}

double Setup::timeShiftForHit(const Hit* hit)
{
  Q_ASSERT(hit->type() == Hit::tof);
  DetectorElement* element = this->element(hit->detId() - hit->channel());
  return static_cast<TOFBar*>(element)->timeShift(hit->channel());
}

TVector3 Setup::configFilePosition(QString group, unsigned short detId) const
{
  assert(m_coordinates);
  QList<QVariant> list = m_coordinates->value(group+"/"+QString::number(detId,16)).toList();
  return TVector3(list[0].toDouble(), list[1].toDouble(), list[2].toDouble());
}

double Setup::configFileAlignmentShift(QString group, unsigned short detId) const
{
  assert(m_settings);
  return m_settings->value(group+"/0x"+QString::number(detId,16)).toDouble();
}

double Setup::configFileTimeShift(unsigned short detId) const
{
  assert(m_settings);
  return m_settings->value("tofTimeShift/0x"+QString::number(detId,16), 1).toDouble();
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
}

