#include "SingleFile.hh"

#include "RunFile.h"
#include "RawEvent.h"
#include "PERDaixFiberModule.h"
#include "PERDaixTRDModule.h"
#include "PERDaixTOFModule.h"
#include "PERDaixPMTModule.h"
#include "SimpleEvent.hh"
#include "CalibrationCollection.hh"
#include "ProgressBar.hh"

#include <iostream>
#include <QDebug>

#ifdef PERDAIX12
#include "ECALModule.h"
#endif

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
  qDeleteAll(m_fiberModules);
  qDeleteAll(m_trdModules);
  qDeleteAll(m_tofModules);
  qDeleteAll(m_pmtModules);
}

void SingleFile::init()
{
  m_runFile = 0;

  // ugly hardcoded
  QList< QPair<quint16, quint16> > hpePairs;
  hpePairs.append(QPair<quint16,quint16>(0x6100,0x7A00));
  hpePairs.append(QPair<quint16,quint16>(0x6400,0x7F00));
  hpePairs.append(QPair<quint16,quint16>(0x6000,0x7B00));
  hpePairs.append(QPair<quint16,quint16>(0x6200,0x7900));
  hpePairs.append(QPair<quint16,quint16>(0x6500,0x7E00));
  hpePairs.append(QPair<quint16,quint16>(0x6700,0x7800));
  hpePairs.append(QPair<quint16,quint16>(0x6600,0x7D00));
  hpePairs.append(QPair<quint16,quint16>(0x3300,0x3000));
  hpePairs.append(QPair<quint16,quint16>(0x3700,0x3100));
  hpePairs.append(QPair<quint16,quint16>(0x6300,0x7C00));
  for (QList< QPair<quint16, quint16> >::iterator iter = hpePairs.begin(); iter != hpePairs.end(); iter++) {
    quint16 value1 = iter->first;
    quint16 value2 = iter->second;
    DetectorID* id1 = DetectorID::Get(value1,DetectorID::TYPE_TRACKER_MODULE);
    DetectorID* id2 = DetectorID::Get(value2,DetectorID::TYPE_TRACKER_MODULE);
    m_fiberModules.push_back(new PERDaixFiberModule(id1,id2));
  }

  m_trdModules.push_back(new PERDaixTRDModule(DetectorID::Get(0x3200, DetectorID::TYPE_TRD_MODULE)));
  m_trdModules.push_back(new PERDaixTRDModule(DetectorID::Get(0x3400, DetectorID::TYPE_TRD_MODULE)));
  m_trdModules.push_back(new PERDaixTRDModule(DetectorID::Get(0x3500, DetectorID::TYPE_TRD_MODULE)));
  m_trdModules.push_back(new PERDaixTRDModule(DetectorID::Get(0x3600, DetectorID::TYPE_TRD_MODULE)));

  m_tofModules.push_back(new PERDaixTOFModule(DetectorID::Get(0x8000, DetectorID::TYPE_TOF_MODULE)));

  m_pmtModules.push_back(new PERDaixPMTModule(DetectorID::Get(0x4000, DetectorID::TYPE_PMT_MODULE)));
}

unsigned int SingleFile::getNumberOfEvents() const
{
  return m_runFile->GetNumberOfEvents() - getNumberOfPedestalEvents() - getNumberOfLedEvents();
}

unsigned int SingleFile::getNumberOfPedestalEvents() const
{
  return m_runFile->GetNumberOfCalibrationEvents();
}

unsigned int SingleFile::getNumberOfLedEvents() const
{
#ifdef PERDAIX12
  return m_runFile->GetNumberOfLedCalibrationEvents();
#endif
  return 0;
}

const RawEvent* SingleFile::getNextRawEvent() const
{
  return (const RawEvent*) m_runFile->ReadNextEvent();
}

void SingleFile::open(QString fileName)
{
  if (m_runFile) {
    m_runFile->Close();
    delete m_runFile;
  }
  m_runFile = new RunFile(fileName, RunFile::MODE_READING);
  int nEvents = getNumberOfEvents();
  std::cout << qPrintable(fileName) << ": contains "
    << nEvents << " regular events, "
    << getNumberOfLedEvents() << " led events and "
    << getNumberOfPedestalEvents() << " pedestal events" << std::endl;
  if (nEvents == 0) {
    qDebug() << "File doesn't contain a valid header. Trying to read as many events as possible. Progress bar will be wrong!";
  }
}

void SingleFile::addPedestalEvent(CalibrationCollection* calibrationCollection, const RawEvent* event)
{
  Q_ASSERT(event);
  QList<DetectorID*> detIDs = event->GetIDs();

  foreach(DetectorID* id, detIDs) {
    if (id->IsTOF())
      continue;
    int blocklength = id->GetDataLength();
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addPedestalHistogram(id->GetID16() | i);
  }

  QMap<DetectorID*, DataBlock*> dataBlockMap;
  foreach(DetectorID* id, detIDs) {
    DataBlock* block = event->GetBlock(id);
    dataBlockMap[id] = block;

    if (id->IsTOF())
      continue;

    // copy calibration information
    int blocklength = id->GetDataLength();
    const quint16* data = 0;
    if (id->IsTracker()) {
      data = ((TrackerDataBlock*) block)->GetRawData();
    }
    else if (id->IsTRD()) {
      data = ((TRDDataBlock*) block)->GetRawData();
    }
    else if (id->IsPMT()) {
      data = ((PMTDataBlock*) block)->GetRawData();
    }
#ifdef PERDAIX12
    else if (id->IsECAL()) {
      data = ((ECALDataBlock*) block)->GetRawData();
    }
#endif
    Q_ASSERT(data);
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addPedestalValue(id->GetID16() | i, data[i]);
  }

  foreach(PERDaixFiberModule* module, m_fiberModules) {
    module->ProcessCalibrationEvent((TrackerDataBlock*) dataBlockMap[module->GetBoardID(PERDaixFiberModule::BOARD_0)]);
    module->ProcessCalibrationEvent((TrackerDataBlock*) dataBlockMap[module->GetBoardID(PERDaixFiberModule::BOARD_1)]);
  }
  foreach(PERDaixTRDModule* module, m_trdModules) {
    module->ProcessCalibrationEvent((TRDDataBlock*) dataBlockMap[module->GetBoardID()]);
  }
  foreach(PERDaixPMTModule* module, m_pmtModules) {
    module->ProcessCalibrationEvent((PMTDataBlock*) dataBlockMap[module->GetBoardID()]);
  }

  qDeleteAll(dataBlockMap);
  delete event;
}

void SingleFile::addLedEvent(CalibrationCollection* calibrationCollection, const RawEvent* event)
{
  QList<DetectorID*> detIDs = event->GetIDs();

  foreach(DetectorID* id, detIDs) {
    if (!id->IsTracker())
      continue;
    int blocklength = id->GetDataLength();
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addLedHistogram(id->GetID16() | i);
  }

  QMap<DetectorID*, DataBlock*> dataBlockMap;
  foreach(DetectorID* id, detIDs) {
    DataBlock* block = event->GetBlock(id);
    dataBlockMap[id] = block;
    if (!id->IsTracker())
      continue;
    int blocklength = id->GetDataLength();
    const quint16* data = ((TrackerDataBlock*) block)->GetRawData();
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addLedValue(id->GetID16() | i, data[i]);
  }

  qDeleteAll(dataBlockMap);
  delete event;
}

const CalibrationCollection* SingleFile::calibrate()
{
  CalibrationCollection* calibrationCollection = new CalibrationCollection;

  std::cout << std::endl << "pedestal events:" << std::endl;
  ProgressBar bar(m_runFile->GetNumberOfCalibrationEvents());
  for (unsigned int i = 0; i < m_runFile->GetNumberOfCalibrationEvents(); i++, bar.next()) {
    const RawEvent* event = static_cast<const RawEvent*>(m_runFile->ReadNextEvent());
    addPedestalEvent(calibrationCollection, event);
  }

  foreach(PERDaixFiberModule* module, m_fiberModules)  module->ProcessCalibrationData();
  foreach(PERDaixTRDModule* module, m_trdModules)  module->ProcessCalibrationData();
  foreach(PERDaixPMTModule* module, m_pmtModules)  module->ProcessCalibrationData();

  std::cout << std::endl << "LED events:" << std::endl;
  bar.restart(getNumberOfLedEvents());
  for (unsigned int i = 0; i < getNumberOfLedEvents(); i++, bar.next()) {
    const RawEvent* event = static_cast<const RawEvent*>(m_runFile->ReadNextEvent());
    addLedEvent(calibrationCollection, event);
  }

  return calibrationCollection;
}

// return pointer to calibration for calibration "whichCali" on hpe/ufe board with id "id"
Calibration* SingleFile::getCalibrationForDetector(DetectorID* id, int whichCali) const
{
  if(id->IsTracker()) {
    foreach(PERDaixFiberModule* module, m_fiberModules) {
      if (module->GetBoardID(PERDaixFiberModule::BOARD_0)->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali);
      else if (module->GetBoardID(PERDaixFiberModule::BOARD_1)->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali+8);
    }
  }
  else if(id->IsTRD()) {
    foreach(PERDaixTRDModule* module, m_trdModules)
      if (module->GetBoardID()->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali);
  }
  else if(id->IsPMT()) {
    foreach(PERDaixPMTModule* module, m_pmtModules)
      if (module->GetBoardID()->GetID16() == id->GetID16())
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
