#ifndef Setup_hh
#define Setup_hh

#include <QMap>
#include <QVector>

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

  static Setup*    instance();
  
public:
  Layer*           layer(double z);
  DetectorElement* element(unsigned short id);

public:
  Layer*           firstLayer();
  DetectorElement* firstElement();
  Layer*           nextLayer();
  DetectorElement* nextElement();

public:
  void             addHitsToLayers(QVector<Hit*> hits);
  void             clearHits();

public:
  void             writeSettings();

private:
  Setup();
  
private:
  void             constructElements();

private:
  static Setup*                          m_instance;

  QSettings*                             m_settings;

  LayerIterator                          m_layerIt;
  ElementIterator                        m_elementIt;

  QMap<double, Layer*>                   m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;

};

#endif /* Setup_hh */
