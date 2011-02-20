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
  SimpleEvent* generateNextSimpleEvent(const SingleFile* file, const MCSingleFile* mcFile);

private:
  const MCEventInformation* generateNextMCEventInformation(const MCSingleFile* mcFile);
  
};

#endif /* Converter_hh */
