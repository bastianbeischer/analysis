#ifndef DetectorElement_hh
#define DetectorElement_hh

class DetectorElement
{
  
public:
  DetectorElement();
  DetectorElement(unsigned short id);
  virtual ~DetectorElement();
  
private:
  unsigned short m_id;
  
};

#endif /* DetectorElement_hh */
