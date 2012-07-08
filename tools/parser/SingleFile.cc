#include "SingleFile.hh"

#include "RunFile.h"
#include "RawEvent.h"
#include "PERDaixFiberModule.h"
#include "PERDaixTRDModule.h"
#include "PERDaixTOFModule.h"
#include "PERDaixPMTModule.h"
#ifdef PERDAIX12
#include "ECALModule.h"
#include "ExternalFiberModule.h"
#endif
#include "SimpleEvent.hh"
#include "CalibrationCollection.hh"
#include "ProgressBar.hh"

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
  qDeleteAll(m_fiberModules);
  qDeleteAll(m_trdModules);
  qDeleteAll(m_tofModules);
  qDeleteAll(m_pmtModules);
#ifdef PERDAIX12
  qDeleteAll(m_ecalModules);
  qDeleteAll(m_externalFiberModules);
#endif
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

#ifdef PERDAIX12
  QList< QPair<quint16, quint16> > vataPairs;
  vataPairs.append(QPair<quint16,quint16>(0x2800, 0x5100));
  vataPairs.append(QPair<quint16,quint16>(0x2900, 0x5000));
  vataPairs.append(QPair<quint16,quint16>(0x2a00, 0x5300));
  vataPairs.append(QPair<quint16,quint16>(0x2b00, 0x5200));
  vataPairs.append(QPair<quint16,quint16>(0x2c00, 0x5500));
  vataPairs.append(QPair<quint16,quint16>(0x2d00, 0x5400));
  vataPairs.append(QPair<quint16,quint16>(0x2e00, 0x5700));
  vataPairs.append(QPair<quint16,quint16>(0x2f00, 0x5600));
  
  vataPairs.append(QPair<quint16,quint16>(0x5800, 0x5c00));
  vataPairs.append(QPair<quint16,quint16>(0x5900, 0x5d00));

  for (QList< QPair<quint16, quint16> >::iterator iter = vataPairs.begin(); iter != vataPairs.end(); iter++) {
    quint16 value1 = iter->first;
    quint16 value2 = iter->second;
    DetectorID* id1 = DetectorID::Get(value1,DetectorID::TYPE_ECAL_MODULE);
    DetectorID* id2 = DetectorID::Get(value2,DetectorID::TYPE_ECAL_MODULE);
    m_ecalModules.push_back(new ECALModule(id1, id2));
  }

  QList< QPair<quint16, quint16> > hpeSpirocPairs;
  hpeSpirocPairs.append(QPair<quint16,quint16>(0x1800, 0x1c00));
  hpeSpirocPairs.append(QPair<quint16,quint16>(0x1900, 0x1d00));
  for (QList< QPair<quint16, quint16> >::iterator iter = hpeSpirocPairs.begin(); iter != hpeSpirocPairs.end(); iter++) {
    quint16 value1 = iter->first;
    quint16 value2 = iter->second;
    DetectorID* id1 = DetectorID::Get(value1, DetectorID::TYPE_EXTERNAL_TRACKER_MODULE);
    DetectorID* id2 = DetectorID::Get(value2, DetectorID::TYPE_EXTERNAL_TRACKER_MODULE);
    m_externalFiberModules.push_back(new ExternalFiberModule(id1, id2));
  }
#endif
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
    //qDebug() << "ID:" << hex << id->GetID16();
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
    } else if (id->IsExternalTracker()) {
      data = ((ExternalTrackerDataBlock*) block)->GetRawData();
    };
#endif
    Q_ASSERT(data);
    for (int i = 0; i < blocklength; i++) {
      calibrationCollection->addPedestalValue(id->GetID16() | i, data[i]);
    }
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
#ifdef PERDAIX12
  foreach(ECALModule* module, m_ecalModules) {
    module->ProcessCalibrationEvent((ECALDataBlock*) dataBlockMap[module->GetBoardID(ECALModule::BOARD_H)]);
    module->ProcessCalibrationEvent((ECALDataBlock*) dataBlockMap[module->GetBoardID(ECALModule::BOARD_Z)]);
  }
  foreach(ExternalFiberModule* module, m_externalFiberModules) {
    module->ProcessCalibrationEvent((ExternalTrackerDataBlock*) dataBlockMap[module->GetBoardID(ExternalFiberModule::BOARD_0)]);
    module->ProcessCalibrationEvent((ExternalTrackerDataBlock*) dataBlockMap[module->GetBoardID(ExternalFiberModule::BOARD_1)]);
  }
#endif
  qDeleteAll(dataBlockMap);
  delete event;
}

void SingleFile::addLedEvent(CalibrationCollection* calibrationCollection, const RawEvent* event)
{
  QList<DetectorID*> detIDs;

  detIDs = event->GetIDs();

  foreach(DetectorID* id, detIDs) {
    if (!id->IsTracker())
      continue;
    int blocklength = id->GetDataLength();
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addLedHistogram(id->GetID16() | i);
  }
  foreach(DetectorID* id, detIDs) {
    DataBlock* block = event->GetBlock(id);
    if (!id->IsTracker())
      continue;
    int blocklength = id->GetDataLength();
    const quint16* data = ((TrackerDataBlock*) block)->GetRawData();
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addLedValue(id->GetID16() | i, data[i]);
  }

#ifdef PERDAIX12
  foreach(DetectorID* id, detIDs) {
    if (!id->IsECAL())
      continue;
    int blocklength = id->GetDataLength();
    Q_ASSERT(blocklength == 64);
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addLedHistogram(id->GetID16() | i);
  }

  foreach(DetectorID* id, detIDs) {
    DataBlock* block = event->GetBlock(id);
    if (!id->IsECAL())
      continue;
    int blocklength = id->GetDataLength();
    Q_ASSERT(blocklength == 64);
    const quint16* data = ((ECALDataBlock*) block)->GetRawData();
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addLedValue(id->GetID16() | i, data[i]);
  }

  foreach(DetectorID* id, detIDs) {
    if (!id->IsExternalTracker())
      continue;
    int blocklength = id->GetDataLength();
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addLedHistogram(id->GetID16() | i);
  }

  foreach(DetectorID* id, detIDs) {
    DataBlock* block = event->GetBlock(id);
    if (!id->IsExternalTracker())
      continue;
    int blocklength = id->GetDataLength();
    const quint16* data = ((ExternalTrackerDataBlock*) block)->GetRawData();
    for (int i = 0; i < blocklength; i++)
      calibrationCollection->addLedValue(id->GetID16() | i, data[i]);
  }
#endif

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
  bar.finish();

  foreach(PERDaixFiberModule* module, m_fiberModules)  module->ProcessCalibrationData();
  foreach(PERDaixTRDModule* module, m_trdModules)  module->ProcessCalibrationData();
  foreach(PERDaixPMTModule* module, m_pmtModules)  module->ProcessCalibrationData();
#ifdef PERDAIX12
  foreach(ECALModule* module, m_ecalModules)  module->ProcessCalibrationData();
  foreach(ExternalFiberModule* module, m_externalFiberModules)  module->ProcessCalibrationData();
#endif

  std::cout << std::endl << "LED events:" << std::endl;
  bar.restart(getNumberOfLedEvents());
  for (unsigned int i = 0; i < getNumberOfLedEvents(); i++, bar.next()) {
    const RawEvent* event = static_cast<const RawEvent*>(m_runFile->ReadNextEvent());
    addLedEvent(calibrationCollection, event);
  }
  bar.finish();

  return calibrationCollection;
}

// return pointer to calibration for calibration "whichCali" on hpe/ufe board with id "id"
Calibration* SingleFile::getCalibrationForDetector(DetectorID* id, int whichCali) const
{
  if (id->IsTracker()) {
    foreach(PERDaixFiberModule* module, m_fiberModules) {
      if (module->GetBoardID(PERDaixFiberModule::BOARD_0)->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali);
      else if (module->GetBoardID(PERDaixFiberModule::BOARD_1)->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali+8);
    }
  }
  else if (id->IsTRD()) {
    foreach(PERDaixTRDModule* module, m_trdModules)
      if (module->GetBoardID()->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali);
  }
  else if (id->IsPMT()) {
    foreach(PERDaixPMTModule* module, m_pmtModules)
      if (module->GetBoardID()->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali);
  }
#ifdef PERDAIX12
  else if (id->IsECAL()) {
    foreach(ECALModule* module, m_ecalModules)
      if (module->GetBoardID(ECALModule::BOARD_H)->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali + ECALModule::BOARD_H);
      else if (module->GetBoardID(ECALModule::BOARD_Z)->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali + ECALModule::BOARD_Z);
  } else if (id->IsExternalTracker()) {
    foreach(ExternalFiberModule* module, m_externalFiberModules) {
      if (module->GetBoardID(ExternalFiberModule::BOARD_0)->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali);
      else if (module->GetBoardID(ExternalFiberModule::BOARD_1)->GetID16() == id->GetID16())
        return (Calibration*) module->GetCalibrations().at(whichCali+8);
    }
  }
#endif
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
