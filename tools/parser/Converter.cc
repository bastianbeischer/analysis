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
  //qSort(detIDs);
  //qDebug() << "detIDs.size() = " << detIDs.size();
  //qDebug() << detIDs;
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
        /*if (cali)
          qDebug("got cali for 0x%x i=%i", id->GetID16(), i);
        else
          qDebug("didn't get cali for 0x%x i=%i", id->GetID16(), i);*/
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
  //generate new mcEventInfo
  MCEventInformation* mcEventInfo = new MCEventInformation();

  //get MCEvent
  const MCEvent* mcEvent = mcFile->getNextMCEvent() ;

  //read MC data
  QVector<MCParticle*> particles = mcEvent->GetParticles();

  //get info on primary and store it
  MCParticle* primarySim = particles.at(0);
  const MCSimpleEventParticle* primary = constructMCSimpleEventParticle (primarySim);
  mcEventInfo->setPrimary(primary);

  //get all secondaries
  for (int i = 1; i < particles.size(); ++i)
  {
    MCParticle* secondaryMC = particles.at(i);
    const MCSimpleEventParticle* secondary = constructMCSimpleEventParticle (secondaryMC);
    mcEventInfo->addSecondary(secondary);
  }


  //get all digis:
  const QVector<const MCSimpleEventDigi*> digis = getAllMCDigis(mcEvent);

  foreach (const MCSimpleEventDigi* digi, digis)
    mcEventInfo->addMcDigis(digi);


  delete mcEvent;

  return mcEventInfo;
}

const MCSimpleEventParticle* Converter::constructMCSimpleEventParticle(MCParticle* mcParticle)
{
  //create MCSimpleEventParticle
  MCSimpleEventParticle* se_particle = new MCSimpleEventParticle();

  //trackID
  se_particle->trackID = mcParticle->GetTrackID();

  //parenttrackID
  se_particle->parentTrackID = mcParticle->GetParentTrackID();

  //pdgID
  se_particle->pdgID = mcParticle->GetPDGID();

  QVector<MCParticle::DetectorHit> hits = mcParticle->GetHits();
  //momentum
  HepGeom::Vector3D<double> mom = hits.at(0).preStep.momentum / CLHEP::GeV;
  se_particle->initialMomentum.SetXYZ(mom.x(), mom.y(), mom.z());
  //position
  HepGeom::Vector3D<double> pos = hits.at(0).preStep.position;
  se_particle->initialPosition.SetXYZ(pos.x(), pos.y(), pos.z());


  //trajectory
  const QVector<HepGeom::Point3D<double> >& trajectory = mcParticle->GetTrajectory();
  for (int i = 0; i < trajectory.size(); ++i){
    const HepGeom::Point3D<double>& point = trajectory.at(i);
    se_particle->trajectory.push_back( TVector3(point.x(), point.y(), point.z()) );
  }

  return se_particle;
}


const QVector<const MCSimpleEventDigi*> Converter::getAllMCDigis(const MCEvent* mcEvent)
{
  //get all digis of MCEvent:
  QVector<MCDigi*> mcDigis = mcEvent->GetDigis();
  //qDebug("read %i digis from raw MC file", mcDigis.size());
  QVector<const MCSimpleEventDigi*> seDigis;
  //loop over all digis and convert:
  for (int i = 0; i < mcDigis.size(); ++i)
  {
    MCDigi* mcDigi = mcDigis.at(i);
    quint16 completeChannelID16bit = static_cast<quint16>(mcDigi->GetDetectorID() & 0xFFFF);
    DetectorID* id = DetectorID::Get(mcDigi->GetDetectorID());

    Hit::ModuleType moduleType = Hit::none;
    if (id->IsTracker())
      moduleType = Hit::tracker;
    else if (id->IsTRD())
      moduleType = Hit::trd;
    else if (id->IsTOF())
      moduleType = Hit::tof;

    //qDebug("got digi type %i with id 0x%x", moduleType, completeChannelID16bit);
    MCSimpleEventDigi* seDigi = new MCSimpleEventDigi(moduleType, completeChannelID16bit);

    //convert all Signals:
    const QVector<MCDigi::Signal>& mcDigiSignals = mcDigi->GetMCValues();
    for (int j = 0; j < mcDigiSignals.size(); ++j)
    {
      const MCDigi::Signal& mcSignal = mcDigiSignals.at(j);
      MCDigiSignal* seSignal = new MCDigiSignal();

      seSignal->trackID = mcSignal._trackId;
      const HepGeom::Point3D<double>& pos = mcSignal._position;
      seSignal->hitPosition.SetXYZ(pos.x(), pos.y(), pos.z());
      seSignal->time = mcSignal._time;
      seSignal->energyDeposition = mcSignal._energyDeposit / CLHEP::keV;

      seDigi->AddSignal(seSignal);
    }

    seDigis << seDigi;
  }

  return seDigis;



}

