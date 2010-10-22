#ifndef Setup_hh
#define Setup_hh

#include <QMap>

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
  void             writeSettings();

private:
  Setup();
  
private:
  static Setup*                          m_instance;

  QSettings*                             m_settings;

  LayerIterator                          m_layerIt;
  ElementIterator                        m_elementIt;

  QMap<double, Layer*>                   m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;

};

#endif /* Setup_hh */
