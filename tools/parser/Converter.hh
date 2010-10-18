#ifndef Converter_hh
#define Converter_hh

class QSettings;
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
  const QSettings*  m_settings;
  
};

#endif /* Converter_hh */
