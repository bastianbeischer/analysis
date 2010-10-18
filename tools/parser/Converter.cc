#include "Converter.hh"

#include "SingleFile.hh"
#include "RawEvent.h"
#include "Hit.hh"
#include "Event.hh"
#include "DetectorID.h"
#include "TrackerDataBlock.h"
#include "TRDDataBlock.h"
#include "TOFDataBlock.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include <TVector3.h>
#include <QSettings>
#include <QDebug>

static double cutTracker = 0;
static double cutTRD = 0;
static double cutTOF = 0;

Converter::Converter(const SingleFile* file) :
  m_file(file),
  m_settings(0)
{
  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIX10_PATH=*"));
  QString path = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    path = entry.split("=").at(1);
    path += "/testing/perdaixGeometry/";
  }

  qDebug() << "Using geometry file: " + path + "perdaix_coordinates.conf";
  m_settings = new QSettings(path+"perdaix_coordinates.conf", QSettings::IniFormat);
}

Converter::~Converter()
{
  delete m_settings;
}

Event* Converter::generateEvent(unsigned int eventNo)
{
  const RawEvent* event = m_file->getRawEvent(eventNo);

  // construct new simple event
  int eventId = event->GetEventID();
  int time = event->GetTime();
  Event* simpleEvent = new Event(eventId, time);

  // loop over all present detector IDs
  QList<DetectorID*> detIDs = event->GetIDs();
  foreach(DetectorID* id, detIDs) {

    //get event data from detector, distinguish types of detectors
    DataBlock* dataBlock = event->GetBlock(id);

    // reserve temporary space
    quint16 nValues = 0;
    if (id->IsTOF())
      nValues = ((TOFDataBlock*) dataBlock)->GetRawDataLength();
    else
      nValues = id->GetDataLength();
    qint16 temp[nValues];

    // get tracker and trd data from block
    int nArrays = 8;
    int nTrd = 2;
    int nMax = 0;
    const quint16* values;
    if (id->IsTracker()) {
      nMax = nArrays;
      values = ((TrackerDataBlock*) dataBlock)->GetRawData();
    }
    else if (id->IsTRD()) {
      nMax = nTrd;
      values = ((TRDDataBlock*) dataBlock)->GetRawData();
    }

    //get calibration for non-tof detectors
    for (int i = 0; i < nMax; i++) {
      if (!id->IsTOF()) {
        Calibration* cali = m_file->getCalibrationForDetector(id, i);
        Q_ASSERT(cali);
        cali->GetAmplitudes(values + i*nValues/nMax, temp + i*nValues/nMax);
      }
    }

    // process data
    TofEvent tofEvent;
    int amplitude;
    TVector3 position;

    unsigned short detId = id->GetID16();

    for (int i = 0; i < nValues; i++) {
      // process tracker and trd
      if (id->IsTracker() || id->IsTRD()) {
        amplitude = static_cast<int>(temp[i]);

        if (id->IsTracker() && amplitude > cutTracker) {
          QList<QVariant> liste = m_settings->value("tracker/"+QString::number(id->GetID16() | i,16)).toList();
          TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
          liste = m_settings->value("trackerback/"+QString::number(id->GetID16() | i,16)).toList();
          TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());

          simpleEvent->addHit(new Hit(Hit::tracker, detId | i, amplitude, pos, counterPos));
        }

        if (id->IsTRD() && amplitude > cutTRD) {
          QList<QVariant> liste = m_settings->value("trd/"+QString::number(id->GetID16() | i,16)).toList();
          TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
          liste = m_settings->value("trdback/"+QString::number(id->GetID16() | i,16)).toList();
          TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());

          simpleEvent->addHit(new Hit(Hit::trd, detId | i, amplitude, pos, counterPos));
        }
      } // tracker, trd

      // add tofhits to tof-event
      else if (id->IsTOF()) {
        const quint32 value = ((TOFDataBlock*) dataBlock)->GetRawData()[i];
        tofEvent.addHit(value);
      }
    } // tracker, trd, tofevent

    // process tof
    for (int channel = 0; channel < 64; channel++) {
      amplitude = tofEvent.timeOverThreshold(channel);
      if (amplitude > cutTOF) {// && amplitude != 400) {
        QList<QVariant> liste = m_settings->value("tof/"+QString::number(id->GetID16() | channel,16)).toList();
        TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
        liste = m_settings->value("tofback/"+QString::number(id->GetID16() | channel,16)).toList();
        TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());

        simpleEvent->addHit(new Hit(Hit::tof, detId | channel, amplitude, pos, counterPos));
      }
    } // tof time over threshold

  } // foreach(DetectorID...)

  return simpleEvent;
}
