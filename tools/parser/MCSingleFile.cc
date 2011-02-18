#include "MCSingleFile.hh"

#include <QString>
#include "RunFile.h"
#include "MCEvent.h"

#include <QDebug>

MCSingleFile::MCSingleFile() :
  m_runFile(0)
{
}

MCSingleFile::MCSingleFile(const QString& fileName) :
  m_runFile(0)
{
  open(fileName);
}

MCSingleFile::~MCSingleFile()
{
  delete m_runFile;
}

void MCSingleFile::open(const QString& fileName)
{
  qDebug("opening MC file %s", qPrintable(fileName));
  m_runFile = new RunFile(fileName, RunFile::MODE_READING);
  //  m_runId = m_runFile->GetRunId();
  //qDebug() << m_runFile->IsGood();

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
}

bool MCSingleFile::isGood(){
  if (m_runFile == 0)
    return false;

  return m_runFile->IsGood();
}

unsigned int MCSingleFile::getNumberOfEvents() const
{
  return m_runFile->GetNumberOfEvents() - m_runFile->GetNumberOfCalibrationEvents();
}

const MCEvent* MCSingleFile::getNextMCEvent() const
{
  return (const MCEvent*) m_runFile->ReadNextEvent();
}
