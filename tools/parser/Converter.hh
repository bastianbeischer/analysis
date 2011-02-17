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
  SimpleEvent* generateNextSimpleEvent(const SingleFile* file);
  
};

#endif /* Converter_hh */
