#include "SingleFile.hh"

#include "RunFile.h"
#include "RawEvent.h"
#include "PERDaixFiberModule.h"
#include "PERDaixTRDModule.h"
#include "PERDaixTOFModule.h"
#include "SimpleEvent.hh"

#include <iostream>
#include <QDebug>

SingleFile::SingleFile()
{
  init();
}

SingleFile::SingleFile(QString fileName)
{
  init();
  open(fileName);
}

SingleFile::~SingleFile()
{
  delete m_runFile;
  cleanupLists();
}

void SingleFile::init()
{
  m_runFile = 0;

  // ugly hardcoded
  m_hpePairs[0x6100]=0x7A00;
  m_hpePairs[0x6400]=0x7F00;
  m_hpePairs[0x6000]=0x7B00;
  m_hpePairs[0x6200]=0x7900;
  m_hpePairs[0x6500]=0x7E00;
  m_hpePairs[0x6700]=0x7800;
  m_hpePairs[0x6600]=0x7D00;
  m_hpePairs[0x3300]=0x3000;
  m_hpePairs[0x3700]=0x3100;
  m_hpePairs[0x6300]=0x7C00;

  m_trdIds.append(0x3200);
  m_trdIds.append(0x3400);
  m_trdIds.append(0x3500);
  m_trdIds.append(0x3600);

  m_tofIds.append(0x8000);
}

void SingleFile::cleanupLists()
{
  foreach(PERDaixFiberModule* module, m_fiberModules) delete module;
  foreach(PERDaixTRDModule* module, m_trdModules)     delete module;
  foreach(PERDaixTOFModule* module, m_tofModules)     delete module;
  m_fiberModules.clear();
  m_trdModules.clear();
  m_tofModules.clear();
}

unsigned int SingleFile::getNumberOfEvents() const
{
  return m_runFile->GetNumberOfEvents() - m_runFile->GetNumberOfCalibrationEvents();
}

const RawEvent* SingleFile::getNextRawEvent() const
{
  return (const RawEvent*) m_runFile->ReadNextEvent();
}

void SingleFile::open(QString fileName)
{
  m_runFile = new RunFile(fileName, RunFile::MODE_READING);
  //  m_runId = m_runFile->GetRunId();
  
  int nCalibrationEvents = m_runFile->GetNumberOfCalibrationEvents();
  int nEvents = m_runFile->GetNumberOfEvents() - nCalibrationEvents;
  std::cout << qPrintable(fileName) << ": contains " << nEvents << " regular events and " << nCalibrationEvents << " calibration events" << std::endl;
  if (nEvents == 0) {
    qDebug() << "SKIPPING FILE!!";
    return;
  }

  for (QMap<quint16, quint16>::iterator iter = m_hpePairs.begin(); iter != m_hpePairs.end(); iter++) {
    quint16 value1 = iter.key();
    quint16 value2 = iter.value();
    DetectorID* id1 = DetectorID::Get(value1,DetectorID::TYPE_TRACKER_MODULE);
    DetectorID* id2 = DetectorID::Get(value2,DetectorID::TYPE_TRACKER_MODULE);
    m_fiberModules.push_back(new PERDaixFiberModule(id1,id2));
  }

  foreach(quint16 id, m_trdIds)
    m_trdModules.push_back(new PERDaixTRDModule(DetectorID::Get(id, DetectorID::TYPE_TRD_MODULE)));

  foreach(quint16 id, m_tofIds)
    m_tofModules.push_back(new PERDaixTOFModule(DetectorID::Get(id, DetectorID::TYPE_TOF_MODULE)));

  calibrate();
}


void SingleFile::calibrate()
{
  for (unsigned int i = 0; i < m_runFile->GetNumberOfCalibrationEvents(); i++) {
    const RawEvent* event = (const RawEvent*) m_runFile->ReadNextEvent();
    QList<DetectorID*> detIDs = event->GetIDs();

    QMap<DetectorID*, DataBlock*> dataBlockMap;
    foreach(DetectorID* id, detIDs)
      dataBlockMap[id] = event->GetBlock(id);

    foreach(PERDaixFiberModule* module, m_fiberModules) {
      module->ProcessCalibrationEvent((TrackerDataBlock*) dataBlockMap[module->GetBoardID(PERDaixFiberModule::BOARD_0)]);
      module->ProcessCalibrationEvent((TrackerDataBlock*) dataBlockMap[module->GetBoardID(PERDaixFiberModule::BOARD_1)]);
    }
    foreach(PERDaixTRDModule* module, m_trdModules) {
      module->ProcessCalibrationEvent((TRDDataBlock*) dataBlockMap[module->GetBoardID()]);
    }
    delete event;
  }

  foreach(PERDaixFiberModule* module, m_fiberModules)  module->ProcessCalibrationData();
  foreach(PERDaixTRDModule* module, m_trdModules)  module->ProcessCalibrationData();
}

// return pointer to calibration for calibration "whichCali" on hpe/ufe board with id "id"
Calibration* SingleFile::getCalibrationForDetector(DetectorID* id, int whichCali) const
{
  if(id->IsTracker()) {
    foreach(PERDaixFiberModule* module, m_fiberModules) {
      if (module->GetBoardID(PERDaixFiberModule::BOARD_0) == id)
        return (Calibration*) module->GetCalibrations().at(whichCali);
      else if (module->GetBoardID(PERDaixFiberModule::BOARD_1) == id)
        return (Calibration*) module->GetCalibrations().at(whichCali+8);
    }
  }
  else if(id->IsTRD()) {
    foreach(PERDaixTRDModule* module, m_trdModules)
      if (module->GetBoardID() == id)
        return (Calibration*) module->GetCalibrations().at(whichCali);
  }

  return 0;
}


unsigned int SingleFile::getStartTime() const
{
  return m_runFile->GetStartTime();
}

unsigned int SingleFile::getStopTime() const
{
  return m_runFile->GetStopTime();
}
