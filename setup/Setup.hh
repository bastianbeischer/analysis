#ifndef Setup_hh
#define Setup_hh

#include "SensorTypes.hh"

#include <QMap>
#include <QVector>
#include <QMutex>

#include <TVector3.h>

class Cluster;
class Hit;
class Layer;
class DetectorElement;
class QSettings;

typedef QMap<double, Layer*>::iterator LayerIterator;
typedef QMap<unsigned short, DetectorElement*>::iterator ElementIterator;

class Setup {
public:
  ~Setup();
  static Setup* instance();

  Layer* layer(double z);
  DetectorElement* element(unsigned short id);
  Layer* firstLayer();
  DetectorElement* firstElement();
  Layer* nextLayer();
  DetectorElement* nextElement();
  QVector<Hit*> generateClusters(const QVector<Hit*>& hits);
  void addHitsToLayers(const QVector<Hit*>& hits);
  void clearHitsFromLayers();
  TVector3 configFilePosition(QString group, unsigned short detId) const;
  double configFileAlignmentShift(QString group, unsigned short detId) const;
  double configFileTimeShift(unsigned short detId) const;
  void writeSettings();
  SensorTypes::Type idToSensor(unsigned short id);

private:
  Setup();
  void construct();
  SensorTypes::Type tofIdToSensor(unsigned short id);

  static Setup* s_instance;
  static QMutex s_mutex;
  QSettings* m_coordinates;
  QSettings* m_settings;
  LayerIterator m_layerIt;
  ElementIterator m_elementIt;
  QMap<double, Layer*> m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;
};

#endif /* Setup_hh */
