#ifndef DataManager_hh
#define DataManager_hh

#include <QList>
#include <QString>

class SingleFile;
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
  void saveAndCloseOutput();

private:
  QList<SingleFile*> m_inputFiles;
  
  QString            m_outputFileName;
  SimpleEvent*       m_currentEvent;
  TFile*             m_outputFile;
  TTree*             m_outputTree;

};

#endif /* DataManager_hh */
