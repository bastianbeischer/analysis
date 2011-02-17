#ifndef Converter_hh
#define Converter_hh

class SimpleEvent;
class SingleFile;

class MCSimpleEvent;
class MCSingleFile;
class MCEventInformation;


class Converter
{
  
public:
  Converter();
  ~Converter();
  
public:
  SimpleEvent* generateSimpleEvent(const SingleFile* file, unsigned int eventNo);
  MCSimpleEvent* generateMCSimpleEvent(const SingleFile* file, const MCSingleFile* mcFile, unsigned int eventNo);

private:
  void fillSimpleEvent(SimpleEvent* simpleevent, const SingleFile* file, unsigned int eventNo);
  MCEventInformation* generateMCEventInformation(const MCSingleFile* mcFile, unsigned int eventNo);
  
};

#endif /* Converter_hh */
