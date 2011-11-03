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
class MagneticField;

typedef QMap<double,Layer*>::const_iterator LayerIterator ;
typedef QMap<unsigned short,DetectorElement*>::const_iterator ElementIterator;

class Setup
{
 
public:
  ~Setup();

  static Setup* instance();
 
public:
  Layer* layer(double z);
  DetectorElement* element(unsigned short id);

  const MagneticField* field() const {return m_field;}

  LayerIterator firstLayer() const {return m_layers.begin();}
  LayerIterator lastLayer() const {return m_layers.end();}
  ElementIterator firstElement() const {return m_elements.begin();}
  ElementIterator lastElement() const {return m_elements.end();}

  unsigned short numberOfLayers() const {return m_layers.size();}
  unsigned short numberOfElements() const {return m_elements.size();}

  TVector3 configFilePosition(QString group, unsigned short detId) const;
  double configFileAlignmentShift(QString group, unsigned short detId) const;
  double configFileTimeShift(unsigned short detId) const;

  void writeSettings();

  SensorTypes::Type sensorForId(unsigned short id);

private:
  Setup();

  void construct();
  Layer* constructLayer(double z);
  DetectorElement* constructElement(unsigned short id);

  SensorTypes::Type tofSensorForId(unsigned short id);
  SensorTypes::Type trackerSensorForId(unsigned short id);

private:
  static Setup* s_instance;
  static QMutex s_mutex;

  QSettings* m_coordinates;
  QSettings* m_settings;

  QMap<double, Layer*> m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;

  MagneticField* m_field;
};

#endif /* Setup_hh */
