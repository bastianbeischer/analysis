#ifndef SingleFile_hh
#define SingleFile_hh

#include <QList>
#include <QMap>

class Calibration;
class DetectorID;
class PERDaixFiberModule;
class PERDaixTOFModule;
class PERDaixTRDModule;
class PERDaixPMTModule;
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
  void calibrate();
  void open(QString fileName);

private:
  RunFile*                      m_runFile;

  QList<PERDaixFiberModule*>    m_fiberModules;
  QList<PERDaixTRDModule*>      m_trdModules;
  QList<PERDaixTOFModule*>      m_tofModules;
  QList<PERDaixPMTModule*>      m_pmtModules;

};

#endif /* SingleFile_hh */
