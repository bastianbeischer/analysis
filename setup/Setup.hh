#ifndef Setup_hh
#define Setup_hh

#include <QMap>

class Layer;
class DetectorElement;

typedef QMap<double,Layer*>::iterator LayerIterator;
typedef QMap<unsigned short,DetectorElement*>::iterator ElementIterator;


class Setup
{
  
public:
  ~Setup();

  static Setup* instance();
  
public:
  Layer*           layer(double z);
  DetectorElement* element(unsigned short id);

public:
  Layer*           firstLayer();
  DetectorElement* firstElement();
  Layer*           nextLayer();
  DetectorElement* nextElement();

private:
  Setup();
  
private:
  static Setup* m_instance;

  LayerIterator   m_layerIt;
  ElementIterator m_elementIt;

  QMap<double, Layer*>                   m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;

};

#endif /* Setup_hh */
