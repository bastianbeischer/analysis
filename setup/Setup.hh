#ifndef Setup_hh
#define Setup_hh

#include <QMap>

class Layer;
class DetectorElement;

class Setup
{
  
public:
  ~Setup();

  static Setup* instance();
  
public:
  Layer*           layer(double z);
  DetectorElement* element(unsigned short id);

private:
  Setup();
  
private:
  static Setup* m_instance;

  QMap<double, Layer*>                   m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;

};

#endif /* Setup_hh */
