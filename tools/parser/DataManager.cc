#include "DataManager.hh"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QProcess>
#include <iostream>

#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>

#include "Converter.hh"
#include "SensorsData.hh"
#include "SimpleEvent.hh"
#include "SingleFile.hh"
#include "MCSingleFile.hh"
#include "DataDescription.hh"

DataManager::DataManager() :
  m_description(0),
  m_sensorsData(0),
  m_atcData(0),
  m_ebassData(0),
  m_outputFileName("output.root"),
  m_currentEvent(0),
  m_outputFile(0),
  m_outputTree(0)
{
  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_PATH=*"));
  QString path = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    path = entry.split("=").at(1);
    path += "/tools/parser/";
  }
  else {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  QString sensorFileName = path + "sensors.root";
  m_sensorsData = new SensorsData(SensorsData::SENSORS, qPrintable(sensorFileName));
  m_atcData = new SensorsData(SensorsData::ATC, qPrintable(sensorFileName));
  m_ebassData = new SensorsData(SensorsData::EBASS, qPrintable(sensorFileName));
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
  m_inputFiles.push_back(file);
  m_description->addRunFile(qPrintable(fileName), hash, file->getNumberOfEvents());

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
  std::cout << "Starting conversion to Simple Events." << std::endl;
  Converter converter;
  int totalNumberOfEvents = 0;
  int currentGlobalEvent = 0;
  foreach(SingleFile* inputFile, m_inputFiles) {
    totalNumberOfEvents += inputFile->getNumberOfEvents();
  }

  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;
  int iFactors = 0;

  for (int i = 0; i < m_inputFiles.size(); ++i) {
    SingleFile* inputFile = m_inputFiles.at(i) ;
    //TODO not very nice to have 2 lists
    MCSingleFile* mcInputFile = m_inputMCFiles.at(i) ;

    unsigned int Nevents = inputFile->getNumberOfEvents();
    for (unsigned int iEvent = 0; iEvent < Nevents; iEvent++) {
      m_currentEvent = converter.generateNextSimpleEvent(inputFile, mcInputFile);
      if (!mcInputFile)
        addSensorData(m_currentEvent);

      m_outputTree->Fill();
      delete m_currentEvent;

      if ( currentGlobalEvent > iFactors*totalNumberOfEvents/100. ) {
        std::cout << "#" << std::flush;
        iFactors++;
      }
      currentGlobalEvent++;
    }
  }

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "Finished conversion." << std::endl;
}

void DataManager::addSensorData(SimpleEvent* event)
{
  int nKeys = m_sensorsData->numberOfKeys();
  const char** keys = m_sensorsData->keys();
  float* values = m_sensorsData->values(event->time());
  for (int iKey = 0; iKey < nKeys; iKey++) {
    event->setSensorData(SensorTypes::convertFromString(keys[iKey]), values[iKey]);
  }

  nKeys = m_atcData->numberOfKeys();
  keys = m_atcData->keys();
  values = m_atcData->values(event->time());
  for (int iKey = 0; iKey < nKeys; iKey++) {
    event->setSensorData(SensorTypes::convertFromString(keys[iKey]), values[iKey]);
  }

  nKeys = m_ebassData->numberOfKeys();
  keys = m_ebassData->keys();
  values = m_ebassData->values(event->time());
  for (int iKey = 0; iKey < nKeys; iKey++) {
    event->setSensorData(SensorTypes::convertFromString(keys[iKey]), values[iKey]);
  }
}
