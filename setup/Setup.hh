#ifndef Setup_hh
#define Setup_hh

#include <QMap>
#include <QVector>

#include <TVector3.h>

class Cluster;
class Hit;
class Layer;
class DetectorElement;
class QSettings;

typedef QMap<double,Layer*>::iterator LayerIterator;
typedef QMap<unsigned short,DetectorElement*>::iterator ElementIterator;

class Setup
{
  
public:
  ~Setup();

  static Setup*     instance();
  
public:
  Layer*            layer(double z);
  DetectorElement*  element(unsigned short id);

public:
  Layer*            firstLayer();
  DetectorElement*  firstElement();
  Layer*            nextLayer();
  DetectorElement*  nextElement();

public:
  QVector<Cluster*> generateClusters(const QVector<Hit*>& hits);
  void              addHitsToLayers(const QVector<Hit*>& hits);
  void              clearHitsFromLayers();

public:
  TVector3          configFilePosition(QString group, unsigned short detId) const;
  double            configFileAlignmentShift(QString group, unsigned short detId) const;

public:
  void              writeSettings();

private:
  Setup();
  
private:
  void              construct();

private:
  static Setup*                          m_instance;

  QSettings*                             m_coordinates;
  QSettings*                             m_settings;

  LayerIterator                          m_layerIt;
  ElementIterator                        m_elementIt;

  QMap<double, Layer*>                   m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;

};

#endif /* Setup_hh */
