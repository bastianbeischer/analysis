#ifndef SingleFile_hh
#define SingleFile_hh

#include <QList>
#include <QMap>

class Calibration;
class DetectorID;
class PERDaixFiberModule;
class PERDaixTOFModule;
class PERDaixTRDModule;
class QString;
class RawEvent;
class RunFile;
class SimpleEvent;

class SingleFile
{
  
public:
  SingleFile();
  SingleFile(QString fileName);
  ~SingleFile();
  
public:
  unsigned int    getNumberOfEvents()                                      const {return m_rawEvents.size();}
  const RawEvent* getRawEvent(unsigned int eventNo)                        const {return m_rawEvents.at(eventNo);}
  Calibration*    getCalibrationForDetector(DetectorID* id, int whichCali) const;
  
private:
  void init();
  void cleanupLists();
  void calibrate();
  void open(QString fileName);

private:
  RunFile*                      m_runFile;

  QMap<quint16, quint16>        m_hpePairs;

  QList<const RawEvent*>        m_calibrationEvents;
  QList<const RawEvent*>        m_rawEvents;

  QList<PERDaixFiberModule*>    m_fiberModules;
  QList<PERDaixTRDModule*>      m_trdModules;
  QList<PERDaixTOFModule*>      m_tofModules;

};

#endif /* SingleFile_hh */
