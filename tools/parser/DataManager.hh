#ifndef DataManager_hh
#define DataManager_hh

#include <QList>
#include <QString>

class DataDescription;
class SensorsData;
class SingleFile;
class MCSingleFile;
class SimpleEvent;
class TTree;
class TFile;

class DataManager
{
  
public:
  DataManager();
  ~DataManager();
  
public:
  void setOutputFileName(QString fileName) {m_outputFileName = fileName;}

  void parseListOfRuns(QString listName);
  void parseSingleRun(QString fileName);

private:
  void openInputFiles(QString listName);
  void addSingleFile(QString fileName);
  void initializeOutput();
  void processFiles();
  void addSensorData(SimpleEvent*);
  void readKeys(SensorsData*, SimpleEvent*);
  void saveAndCloseOutput();

private:
  QList<SingleFile*> m_inputFiles;
  QList<MCSingleFile*> m_inputMCFiles;
  
  DataDescription*   m_description;
  SensorsData*       m_sensorsData;
  SensorsData*       m_atcData;
  SensorsData*       m_ebassData;
  SensorsData*       m_testbeamData;
  QString            m_outputFileName;
  SimpleEvent*       m_currentEvent;
  TFile*             m_outputFile;
  TTree*             m_outputTree;

};

#endif /* DataManager_hh */
