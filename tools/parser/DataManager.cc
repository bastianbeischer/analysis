#include "DataManager.hh"

#include <QDebug>
#include <QFile>
#include <QString>
#include <iostream>

#include <TTree.h>
#include <TFile.h>

#include "Converter.hh"
#include "SimpleEvent.hh"
#include "SingleFile.hh"

DataManager::DataManager() :
  m_outputFileName("output.root"),
  m_currentEvent(0),
  m_outputFile(0),
  m_outputTree(0)
{
}

DataManager::~DataManager()
{
  foreach(SingleFile* file, m_inputFiles)
    delete file;
  m_inputFiles.clear();

  delete m_outputFile;
}

void DataManager::parseListOfRuns(QString listName)
{
  openInputFiles(listName);
  initializeOutput();
  processFiles();
  saveAndCloseOutput();
}

void DataManager::parseSingleRun(QString fileName)
{
  addSingleFile(fileName);
  initializeOutput();
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
  std::cout << "Processing: " << qPrintable(fileName) << std::endl;
  m_inputFiles.push_back(new SingleFile(qPrintable(fileName)));
}

void DataManager::initializeOutput()
{
  m_outputFile = new TFile(qPrintable(m_outputFileName), "RECREATE");
  m_outputTree = new TTree("EventTree", "tree with simple events");
  m_outputTree->Branch("SimpleEvent", "SimpleEvent", &m_currentEvent); 
}

void DataManager::saveAndCloseOutput()
{
  m_outputFile->cd();
  m_outputTree->Write();
  m_outputFile->Close();
}

void DataManager::processFiles()
{
  std::cout << "Starting conversion to Events." << std::endl;
  foreach(SingleFile* inputFile, m_inputFiles) {
    Converter converter(inputFile);
    for (unsigned int iEvent = 0; iEvent < inputFile->getNumberOfEvents(); iEvent++) {
      m_currentEvent = converter.generateSimpleEvent(iEvent);
      m_outputTree->Fill();
    }
  }
  std::cout << "Finished conversion." << std::endl;
}
