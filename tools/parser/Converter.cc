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
#include "DetectorElement.hh"

#include "MCSingleFile.hh"
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
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    unsigned short elementId = element->id();
    unsigned short nChannels = element->nChannels();
    QString group;
    if (element->type() == DetectorElement::tracker)
      group = "tracker";
    if (element->type() == DetectorElement::trd)
      group = "trd";
    if (element->type() == DetectorElement::tof)
      group = "tof";
    for (int i = 0; i < nChannels; i++) {
      unsigned short id = elementId | i;
      m_positions[id] = setup->configFilePosition(group, id);
      m_counterPositions[id] = setup->configFilePosition(group+"back", id);
    }
  }
}

Converter::~Converter()
{
}

SimpleEvent* Converter::generateNextSimpleEvent(const SingleFile* file, const MCSingleFile* mcFile)
{
  const RawEvent* event = file->getNextRawEvent();

  // fill simple event basics
  unsigned int eventId = event->GetEventID();
  unsigned int runStartTime = file->getStartTime(); // convert ms to s for
  unsigned int eventTime = event->GetTime();

  SimpleEvent* simpleEvent = new SimpleEvent(eventId, runStartTime, eventTime, mcFile? SimpleEvent::MonteCarlo : SimpleEvent::Data);

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

    for (int i = 0; i < nValues; i++) {

      if (id->IsTracker()) {
        int amplitude = static_cast<int>(temp[i]);

        TVector3& pos = m_positions[detId | i];
        TVector3& counterPos = m_counterPositions[detId | i];

        simpleEvent->addHit(new Hit(Hit::tracker, detId | i, amplitude, pos, counterPos));
      } // tracker

      else if (id->IsTRD()) {
        int amplitude = static_cast<int>(temp[i]);

        TVector3& pos = m_positions[detId | i];
        TVector3& counterPos = m_counterPositions[detId | i];

        simpleEvent->addHit(new Hit(Hit::trd, detId | i, amplitude, pos, counterPos));
      } // trd

      else if (id->IsTOF()) {
        const quint32 value = ((TOFDataBlock*) dataBlock)->GetRawData()[i];
        int channel = TOFSipmHit::channelFromData(value);

        unsigned short bar = tdcChannelToBar[channel] << 2;
        unsigned short sipm = tdcChannelToSipm[channel];

        unsigned short fullDetId = detId | bar | sipm;

        TVector3& pos = m_positions[fullDetId];
        TVector3& counterPos = m_counterPositions[fullDetId];

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

  delete event;

  if (mcFile) {
    const MCEventInformation* mcInfo = generateNextMCEventInformation(mcFile);
    simpleEvent->setMCInformation(mcInfo);
  }

  return simpleEvent;
}


const MCEventInformation* Converter::generateNextMCEventInformation(const MCSingleFile* mcFile)
{
  //get MCEvent
  const MCEvent* mcEvent = mcFile->getNextMCEvent() ;

  //read MC data
  QVector<MCParticle*> particles = mcEvent->GetParticles();

  //get info on primary
  MCParticle* primary = particles.at(0);
  int pdgID = primary->GetPDGID();

  QVector<MCParticle::DetectorHit> primaryHits = primary->GetHits();

  HepGeom::Vector3D<double> primaryMom = primaryHits.at(0).preStep.momentum;

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
  mcEventInfo->setPdgId(pdgID);
  mcEventInfo->setInitialMomentum(mc_primaryMomentum);
  mcEventInfo->setTrajectory(traj);

  delete mcEvent;

  return mcEventInfo;
}


