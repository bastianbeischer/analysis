#ifndef Converter_hh
#define Converter_hh

class SimpleEvent;
class SingleFile;

class Converter
{
  
public:
  Converter();
  ~Converter();
  
public:
  SimpleEvent* generateSimpleEvent(const SingleFile* file, unsigned int eventNo);
  
};

#endif /* Converter_hh */
