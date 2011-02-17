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
#include "Setup.hh"

#include "MCSingleFile.hh"
#include "MCSimpleEvent.hh"
#include "MCEventInformation.hh"
#include "MCEvent.h"
#include "MCParticle.h"
#include "MCDigi.h"

#include "CLHEP/Units/SystemOfUnits.h"
#include <CLHEP/Geometry/Point3D.h>
#include <CLHEP/Geometry/Vector3D.h>

#include <TVector3.h>
#include <QDebug>

#include <iostream>

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

Converter::Converter()
{
}

Converter::~Converter()
{
}

SimpleEvent* Converter::generateSimpleEvent(const SingleFile* file, unsigned int eventNo)
{
  const RawEvent* event = file->getRawEvent(eventNo);

  // construct new simple event
  unsigned int eventId = event->GetEventID();
  unsigned int runStartTime = file->getStartTime(); // convert ms to s for 
  unsigned int eventTime = event->GetTime();
  SimpleEvent* simpleEvent = new SimpleEvent(eventId, runStartTime, eventTime, SimpleEvent::RawData);

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
        Calibration* cali = file->getCalibrationForDetector(id, i);
        Q_ASSERT(cali);
        cali->GetAmplitudes(values + i*nValues/nMax, temp + i*nValues/nMax);
      }
    }

    // process data
    unsigned short detId = id->GetID16();
    std::map<unsigned short, TOFSipmHit*> tofHitMap; // maps channel to sipm hits
    Setup* setup = Setup::instance();

    for (int i = 0; i < nValues; i++) {

      if (id->IsTracker()) {
        int amplitude = static_cast<int>(temp[i]);

        TVector3 pos = setup->configFilePosition("tracker", detId | i);
        TVector3 counterPos = setup->configFilePosition("trackerback", detId | i);

        simpleEvent->addHit(new Hit(Hit::tracker, detId | i, amplitude, pos, counterPos));
      } // tracker

      else if (id->IsTRD()) {
        int amplitude = static_cast<int>(temp[i]);

        TVector3 pos = setup->configFilePosition("trd", detId | i);
        TVector3 counterPos = setup->configFilePosition("trdback", detId | i);

        simpleEvent->addHit(new Hit(Hit::trd, detId | i, amplitude, pos, counterPos));
      } // trd

      else if (id->IsTOF()) {
        const quint32 value = ((TOFDataBlock*) dataBlock)->GetRawData()[i];
        int channel = TOFSipmHit::channelFromData(value);

        unsigned short bar = tdcChannelToBar[channel] << 2;
        unsigned short sipm = tdcChannelToSipm[channel];

        unsigned short fullDetId = detId | bar | sipm;

        TVector3 pos = setup->configFilePosition("tof", fullDetId);
        TVector3 counterPos = setup->configFilePosition("tofback", fullDetId);


        if (!tofHitMap[channel]) {
          tofHitMap[channel] = new TOFSipmHit(fullDetId, pos, counterPos);
          simpleEvent->addHit(tofHitMap[channel]);
        }
        tofHitMap[channel]->addLevelChange(value);
      } // tof

    } // all hits

    std::map<unsigned short, TOFSipmHit*>::iterator tofHitIt = tofHitMap.begin();
    for (; tofHitIt != tofHitMap.end(); ++tofHitIt)
      tofHitIt->second->processTDCHits();
  } // foreach(DetectorID...)

  return simpleEvent;
}


MCEventInformation* Converter::generateMCEventInformation(const MCSingleFile* mcFile, unsigned int eventNo){

  //get MCEvent
  const MCEvent* mcEvent = mcFile->getMCEvent(eventNo) ;

  //read MC data
  QVector<MCParticle*> particles = mcEvent->GetParticles();
  QVector<MCDigi*> digis = mcEvent->GetDigis();

  //get info on primary
  MCParticle* primary = particles.at(0);
  QVector<MCParticle::DetectorHit> primaryHits = primary->GetHits();

  HepGeom::Vector3D<double> primaryMom = primaryHits.at(0).preStep.momentum;


  //convert into data structures used by SimpleEvents
  TVector3 mcI_primaryMomentum(primaryMom.x(), primaryMom.y(), primaryMom.z())  ;

  //create MC Information Object
  MCEventInformation* mcEventInfo = new MCEventInformation();

  //fill it with data
  mcEventInfo->InitialMom(mcI_primaryMomentum);

  return mcEventInfo;
}


MCSimpleEvent* Converter::generateMCSimpleEvent(const SingleFile* file, const MCSingleFile* mcFile, unsigned int eventNo){

  SimpleEvent* simpleEvent = generateSimpleEvent(file, eventNo);
  MCEventInformation* mcEventInfo = generateMCEventInformation(mcFile, eventNo);

  //TODO maybe find a better solution:
  MCSimpleEvent* mcSimpleEvent = new MCSimpleEvent(*simpleEvent);
  delete simpleEvent;

  //MCEventInformation is now owned by MCSimpleEvent:
  mcSimpleEvent->MCInformation(mcEventInfo);

  qDebug() << mcEventInfo->InitialMom().x() << mcEventInfo->InitialMom().y() << mcEventInfo->InitialMom().z();

  return mcSimpleEvent;
}
