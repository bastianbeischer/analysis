#include "Converter.hh"

#include "SingleFile.hh"
#include "RawEvent.h"
#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "SimpleEvent.hh"
#include "DetectorID.h"
#include "TrackerDataBlock.h"
#include "TRDDataBlock.h"
#include "TOFDataBlock.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include <TVector3.h>
#include <QSettings>
#include <QDebug>

const int tdcChannelToBar[64] = {
  5,  5,  1,  1,  4,  4,  0,  0,  7,  7,  3,  3,  6,  6,  2,  2,
  6,  6,  2,  2,  7,  7,  3,  3,  4,  4,  0,  0,  5,  5,  1,  1,
  15, 15, 11, 11, 14, 14, 10, 10, 13, 13, 9,  9,  12, 12, 8,  8,
  12, 12, 8,  8,  13, 13, 9,  9,  14, 14, 10, 10, 15, 15, 11, 11
};

const int tdcChannelToSipm[64] = {
  2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0
};

Converter::Converter(const SingleFile* file) :
  m_file(file),
  m_settings(0)
{
  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_PATH=*"));
  QString path = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    path = entry.split("=").at(1);
    path += "/tools/parser/";
  }

  qDebug() << "Using geometry file: " + path + "perdaix_coordinates.conf";
  m_settings = new QSettings(path+"perdaix_coordinates.conf", QSettings::IniFormat);
}

Converter::~Converter()
{
  delete m_settings;
}

SimpleEvent* Converter::generateSimpleEvent(unsigned int eventNo)
{
  const RawEvent* event = m_file->getRawEvent(eventNo);

  // construct new simple event
  int eventId = event->GetEventID();
  int time = event->GetTime();
  SimpleEvent* simpleEvent = new SimpleEvent(eventId, time);

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
    unsigned short detId = id->GetID16();
    std::map<unsigned short, TOFSipmHit*> tofHitMap; // maps channel to sipm hits

    for (int i = 0; i < nValues; i++) {

      if (id->IsTracker()) {
        int amplitude = static_cast<int>(temp[i]);

        QList<QVariant> liste = m_settings->value("tracker/"+QString::number(detId | i,16)).toList();
        TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
        liste = m_settings->value("trackerback/"+QString::number(detId | i,16)).toList();
        TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());

        simpleEvent->addHit(new Hit(Hit::tracker, detId | i, amplitude, pos, counterPos));
      } // tracker

      else if (id->IsTRD()) {
        int amplitude = static_cast<int>(temp[i]);

        QList<QVariant> liste = m_settings->value("trd/"+QString::number(detId | i,16)).toList();
        TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
        liste = m_settings->value("trdback/"+QString::number(detId | i,16)).toList();
        TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());

        simpleEvent->addHit(new Hit(Hit::trd, detId | i, amplitude, pos, counterPos));
      } // trd

      else if (id->IsTOF()) {
        const quint32 value = ((TOFDataBlock*) dataBlock)->GetRawData()[i];
        int channel = TOFSipmHit::channelFromData(value);

        QList<QVariant> liste = m_settings->value("tof/"+QString::number(detId | channel,16)).toList();
        TVector3 pos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());
        liste = m_settings->value("tofback/"+QString::number(detId | channel,16)).toList();
        TVector3 counterPos(liste[0].toDouble(), liste[1].toDouble(), liste[2].toDouble());

        unsigned short bar = tdcChannelToBar[channel] << 2;
        unsigned short sipm = tdcChannelToSipm[channel];

        if (!tofHitMap[channel]) {
          tofHitMap[channel] = new TOFSipmHit(detId | bar | sipm, pos, counterPos);
          simpleEvent->addHit(tofHitMap[channel]);
        }
        tofHitMap[channel]->addLevelChange(value);
      } // tof

    } // all hits

  } // foreach(DetectorID...)

  return simpleEvent;
}
