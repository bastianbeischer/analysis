#include "MCSingleFile.hh"

#include <QString>
#include "RunFile.h"
#include "MCEvent.h"

#include <QDebug>

MCSingleFile::MCSingleFile()
{
  init();
}

MCSingleFile::MCSingleFile(QString fileName)
{
  init();
  open(fileName);
}

MCSingleFile::~MCSingleFile()
{
  delete m_runFile;
  cleanupLists();
}

void MCSingleFile::init()
{
  m_runFile = 0;
}

void MCSingleFile::cleanupLists()
{
  foreach(const MCEvent* mcEvent, m_MCEvents) delete mcEvent;
  m_MCEvents.clear();
}

void MCSingleFile::open(QString fileName)
{
  qDebug("opening MC file %s", qPrintable(fileName));
  m_runFile = new RunFile(fileName, RunFile::MODE_READING);
  //  m_runId = m_runFile->GetRunId();
  qDebug() << m_runFile->IsGood();

  int nCalibrationEvents = m_runFile->GetNumberOfCalibrationEvents();
  int nEvents = m_runFile->GetNumberOfEvents() - nCalibrationEvents;
  std::cout << qPrintable(fileName) << ": contains " << nEvents << " regular events and " << nCalibrationEvents << " calibration events" << std::endl;
  if (nEvents == 0) {
    qDebug("SKIPPING FILE!!");
    return;
  }

  //skip calis
  for (int i=0; i<nCalibrationEvents; ++i)
    m_runFile->ReadNextEvent();

  for (int i=0; i<nEvents; ++i)
    m_MCEvents << static_cast<MCEvent*> (m_runFile->ReadNextEvent());




}

bool MCSingleFile::IsGood(){
  if (m_runFile == 0)
    return false;

  return m_runFile->IsGood();
}
