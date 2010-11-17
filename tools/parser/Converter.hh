#ifndef Converter_hh
#define Converter_hh

class RawEvent;
class SimpleEvent;
class SingleFile;

class Converter
{
  
public:
  Converter(const SingleFile* file);
  ~Converter();
  
public:
  SimpleEvent* generateSimpleEvent(unsigned int eventNo);

private:
  const SingleFile* m_file;
  
};

#endif /* Converter_hh */
