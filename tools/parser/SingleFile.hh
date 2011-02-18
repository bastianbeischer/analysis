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
  unsigned int    getNumberOfEvents() const;
  const RawEvent* getNextRawEvent() const;
  Calibration*    getCalibrationForDetector(DetectorID* id, int whichCali) const;
  unsigned int    getStartTime() const;
  unsigned int    getStopTime() const;

private:
  void init();
  void cleanupLists();
  void calibrate();
  void open(QString fileName);

private:
  RunFile*                      m_runFile;

  QMap<quint16, quint16>        m_hpePairs;
  QList<quint16>                m_trdIds;
  QList<quint16>                m_tofIds;

  QList<PERDaixFiberModule*>    m_fiberModules;
  QList<PERDaixTRDModule*>      m_trdModules;
  QList<PERDaixTOFModule*>      m_tofModules;

};

#endif /* SingleFile_hh */
