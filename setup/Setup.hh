#ifndef Setup_hh
#define Setup_hh

class Setup
{
  
public:
  static Setup* instance();
  ~Setup();
  
private:
  Setup();
  
private:
  static Setup* m_instance;
  
};

#endif /* Setup_hh */
