#ifndef Setup_hh
#define Setup_hh

#include <QMap>

class DetectorElement;

class Setup
{
  
public:
  ~Setup();

  static Setup* instance();
  
  DetectorElement* element(unsigned short id);

private:
  Setup();
  
private:
  static Setup* m_instance;

  QMap<unsigned short, DetectorElement*> m_elements;

};

#endif /* Setup_hh */
