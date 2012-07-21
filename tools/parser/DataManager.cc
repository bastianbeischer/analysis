#include "DataManager.hh"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QRegExp>
#include <iostream>

#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>

#include "Helpers.hh"
#include "Converter.hh"
#include "PreAnalysis.hh"
#include "SensorsData.hh"
#include "SimpleEvent.hh"
#include "SingleFile.hh"
#include "MCSingleFile.hh"
#include "DataDescription.hh"

#include "SettingsManager.hh"
#include "Settings.hh"
#include "Helpers.hh"
#include "ProgressBar.hh"

DataManager::DataManager() :
  m_description(0),
  m_sensorsData(0),
  m_atcData(0),
  m_ebassData(0),
  m_testbeam11Data(0),
  m_testbeam12Data(0),
  m_outputFileName("output.root"),
  m_currentEvent(0),
  m_outputFile(0),
  m_outputTree(0)
{
  QString path = Helpers::analysisPath();
  QString sensorFileName = path + "/tools/parser/kiruna.root";
  QString testbeam11FileName = path + "/tools/parser/testbeam11.root";
  QString testbeam12FileName = path + "/tools/parser/testbeam12.root";
  m_sensorsData = new SensorsData(SensorsData::SENSORS, qPrintable(sensorFileName));
  m_atcData = new SensorsData(SensorsData::ATC, qPrintable(sensorFileName));
  m_ebassData = new SensorsData(SensorsData::EBASS, qPrintable(sensorFileName));
  m_testbeam11Data = new SensorsData(SensorsData::SENSORS, qPrintable(testbeam11FileName));
  m_testbeam12Data = new SensorsData(SensorsData::SENSORS, qPrintable(testbeam12FileName));
}

DataManager::~DataManager()
{
  foreach(SingleFile* file, m_inputFiles)
    delete file;
  m_inputFiles.clear();

  foreach(MCSingleFile* file, m_inputMCFiles)
    delete file;
  m_inputMCFiles.clear();

  delete m_sensorsData;
  delete m_atcData;
  delete m_ebassData;
  delete m_testbeam11Data;
  delete m_testbeam12Data;
  delete m_outputFile;
}

void DataManager::parseListOfRuns(QString listName)
{
  initializeOutput();
  openInputFiles(listName);
  processFiles();
  saveAndCloseOutput();
}

void DataManager::parseSingleRun(QString fileName)
{
  initializeOutput();
  addSingleFile(fileName);
  processFiles();
  saveAndCloseOutput();
}

void DataManager::openInputFiles(QString listName)
{
  QFile list(listName);
  if (!list.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::cout << "Error opening file: " << qPrintable(listName) << std::endl;
    return;
  }

  QTextStream in(&list);
  int nFiles = 0;
  while (!in.atEnd()) {
    QString line = in.readLine();
    addSingleFile(line);
    nFiles++;
  }

  std::cout << "Finished reading " << nFiles << " input files."<< std::endl;
  //  std::cout << "Total number of events: " << m_rawEvents.size() << std::endl;
}

void DataManager::addSingleFile(QString fileName)
{
  const std::string& hash = DataDescription::calculateSoftwareVersionHash();
  std::cout << "Processing: " << qPrintable(fileName) <<  " with SHA1 hash " << hash << std::endl;
  SingleFile* file = new SingleFile(qPrintable(fileName));
  const CalibrationCollection* calibrationCollection = file->calibrate();
  m_inputFiles.push_back(file);
  int nLedEvents = 0;
#ifdef PERDAIX12
  nLedEvents = file->getNumberOfLedEvents();
#endif
  m_description->addRunFile(qPrintable(fileName), hash, file->getNumberOfEvents(),
    file->getNumberOfPedestalEvents(), nLedEvents, calibrationCollection);

  //try to open a matching mc file:
  QString mcFileName = fileName.replace(QRegExp("\\.dat$"), "_MC.dat");
  if (QFile::exists(mcFileName)) {
    qDebug("found MC file");
    MCSingleFile* mcFile = new MCSingleFile(qPrintable(mcFileName));
    m_inputMCFiles.push_back(mcFile);
  } else {
    m_inputMCFiles.push_back(0);
  }
}

void DataManager::initializeOutput()
{
  m_outputFile = new TFile(qPrintable(m_outputFileName), "RECREATE");
  m_outputTree = new TTree("SimpleEventTree", "tree with simple events");
  m_description = new DataDescription;
  m_description->setSoftwareVersionHash();
  m_outputTree->GetUserInfo()->Add(m_description);
  m_outputTree->Branch("event", "SimpleEvent", &m_currentEvent);
}

void DataManager::saveAndCloseOutput()
{
  m_outputFile->Write();
  m_outputFile->Close();
}


void DataManager::processFiles()
{
  Converter converter;
  PreAnalysis preAna;
  int totalNumberOfEvents = 0;
  foreach(SingleFile* inputFile, m_inputFiles)
    totalNumberOfEvents += inputFile->getNumberOfEvents();

  std::cout << std::endl << "particle events:" << std::endl;
  ProgressBar bar(totalNumberOfEvents);
  for (int i = 0; i < m_inputFiles.size(); ++i, bar.next()) {
    SingleFile* inputFile = m_inputFiles.at(i) ;
    MCSingleFile* mcInputFile = m_inputMCFiles.at(i) ;
    m_currentEvent = converter.generateNextSimpleEvent(inputFile, mcInputFile);
    while (m_currentEvent) {
      bar.next();
      if (!mcInputFile)
        addSensorData(m_currentEvent);
      m_currentEvent = preAna.generateCompressedEvent(m_currentEvent);
      m_outputTree->Fill();
      delete m_currentEvent;
      m_currentEvent = converter.generateNextSimpleEvent(inputFile, mcInputFile);
    }
  }
  bar.finish();
  std::cout << "Finished conversion." << std::endl;
}

void DataManager::addSensorData(SimpleEvent* event)
{
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  if (settings && settings->situation() == Enums::Testbeam11) {
    readKeys(m_testbeam11Data, event);
  }
  else if (settings && settings->situation() == Enums::Testbeam12) {
    readKeys(m_testbeam12Data, event);
  }
  else {
    readKeys(m_sensorsData, event);
    readKeys(m_atcData, event);
    readKeys(m_ebassData, event);
  }
}

void DataManager::readKeys(SensorsData* data, SimpleEvent* event)
{
  if (data->good()) {
    int nKeys = data->numberOfKeys();
    const char** keys = data->keys();
    float* values = data->values(event->time());
    for (int iKey = 0; iKey < nKeys; iKey++) {
      event->setSensorData(SensorTypes::convertFromString(keys[iKey]), values[iKey]);
    }
  }
}
