#ifndef Setup_hh
#define Setup_hh

#include <QMap>
#include <QVector>
#include <QMutex>

#include <TVector3.h>

class Cluster;
class Hit;
class Layer;
class DetectorElement;
class QSettings;

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

  LayerIterator firstLayer() const {return m_layers.begin();}
  LayerIterator lastLayer() const {return m_layers.end();}
  ElementIterator firstElement() const {return m_elements.begin();}
  ElementIterator lastElement() const {return m_elements.end();}

  unsigned short numberOfLayers() const {return m_layers.size();}
  unsigned short numberOfElements() const {return m_elements.size();}

  QVector<Hit*> generateClusters(const QVector<Hit*>& hits);
  void addHitsToLayers(const QVector<Hit*>& hits);
  void clearHitsFromLayers();

  TVector3 configFilePosition(QString group, unsigned short detId) const;
  double configFileAlignmentShift(QString group, unsigned short detId) const;
  double configFileTimeShift(unsigned short detId) const;

  void writeSettings();

private:
  Setup();
  
private:
  void construct();

private:
  static Setup* s_instance;
  static QMutex s_mutex;

  QSettings* m_coordinates;
  QSettings* m_settings;

  QMap<double, Layer*> m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;

};

#endif /* Setup_hh */
