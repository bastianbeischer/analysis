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
  SimpleEvent* simpleEvent = new SimpleEvent();
  simpleEvent->contentType(SimpleEvent::RawData);
  fillSimpleEvent(simpleEvent, file, eventNo);
  return simpleEvent;
}


MCSimpleEvent* Converter::generateMCSimpleEvent(const SingleFile* file, const MCSingleFile* mcFile, unsigned int eventNo){

  //generate MCSimpleEvent and fill it with data:
  MCSimpleEvent* mcSimpleEvent = new MCSimpleEvent();
  mcSimpleEvent->contentType(SimpleEvent::MCRawData);
  fillSimpleEvent(mcSimpleEvent, file, eventNo);

  //get MCEventInformation
  MCEventInformation* mcEventInfo = generateMCEventInformation(mcFile, eventNo);
  //MCEventInformation is now owned by MCSimpleEvent:
  mcSimpleEvent->MCInformation(mcEventInfo);

  qDebug() << mcEventInfo->InitialMom().x() << mcEventInfo->InitialMom().y() << mcEventInfo->InitialMom().z();

  return mcSimpleEvent;
}



void Converter::fillSimpleEvent(SimpleEvent* simpleEvent, const SingleFile* file, unsigned int eventNo){

  const RawEvent* event = file->getRawEvent(eventNo);

  // fill simple event basics
  unsigned int eventId = event->GetEventID();
  unsigned int runStartTime = file->getStartTime(); // convert ms to s for
  unsigned int eventTime = event->GetTime();

  simpleEvent->eventId(eventId);
  simpleEvent->runStartTime(runStartTime);
  simpleEvent->eventTime(eventTime);

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

}


MCEventInformation* Converter::generateMCEventInformation(const MCSingleFile* mcFile, unsigned int eventNo){

  //get MCEvent
  const MCEvent* mcEvent = mcFile->getMCEvent(eventNo) ;

  qDebug() << mcEvent;

  //read MC data
  QVector<MCParticle*> particles = mcEvent->GetParticles();

  //get info on primary
  MCParticle* primary = particles.at(0);
  int pdgID = primary->GetPDGID();

  QVector<MCParticle::DetectorHit> primaryHits = primary->GetHits();

  HepGeom::Vector3D<double> primaryMom = primaryHits.at(0).preStep.momentum;

  foreach (HepGeom::Point3D<double> point, primary->GetTrajectory())
    qDebug() << "\t" << point.x() << point.y() << point.z();

  QVector<HepGeom::Point3D<double> > mcTrajectory = primary->GetTrajectory();




  //convert into data structures used by SimpleEvents
  TVector3 mc_primaryMomentum(primaryMom.x(), primaryMom.y(), primaryMom.z())  ;

  std::vector<TVector3> traj;
  for (int i = 0; i < mcTrajectory.size(); ++i){
    const HepGeom::Point3D<double>& point = mcTrajectory.at(i);
    traj.push_back( TVector3(point.x(), point.y(), point.z()) );
  }

  //create MC Information Object
  MCEventInformation* mcEventInfo = new MCEventInformation();

  //fill it with data
  mcEventInfo->PDGid(pdgID);
  mcEventInfo->InitialMom(mc_primaryMomentum);
  mcEventInfo->Trajectory(traj);

  return mcEventInfo;
}


