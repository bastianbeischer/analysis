#include "EventWriter.hh"

#include <QCoreApplication>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

#include "EventReader.hh"
#include "EventQueue.hh"
#include "SimpleEvent.hh"
#include "DataDescription.hh"
#include "DataChain.hh"

#include <iostream>

EventWriter::EventWriter(QObject* parent) :
  QThread(parent),
  EventDestination(),
  m_mutex(),
  m_finished(true),
  m_queue(new EventQueue)
{
}

EventWriter::~EventWriter()
{
  delete m_file;
  delete m_queue;
}

void EventWriter::init(QString filename)
{
  m_file = new TFile(qPrintable(filename), "RECREATE");
  m_tree = new TTree("SimpleEventTree", "tree with simple events");
  m_tree->SetDirectory(m_file);
  m_event = 0;
  m_tree->Branch("event", "SimpleEvent", &m_event);

  m_description = new DataDescription;
  m_description->setSoftwareVersionHash();
}

void EventWriter::addInputFileToDescription(QString filename)
{
  TFile file(qPrintable(filename), "READ");
  TTree* tree = (TTree*) file.Get("SimpleEventTree");
  DataDescription* inputDesc = (DataDescription*) tree->GetUserInfo()->At(0);
  int nEvents = 0;
  int nCalibrationEvents = 0 ;
  int nRuns = inputDesc->numberOfRuns();
  for (int i = 0; i < nRuns; i++) {
    nEvents += inputDesc->numberOfEventsInRunFile(i);
    nCalibrationEvents += inputDesc->numberOfCalibrationEventsInRunFile(i);
  }
  m_description->addRunFile(filename.toStdString(), inputDesc->softwareVersionHash(), nEvents, nCalibrationEvents);
}

void EventWriter::start()
{
  m_mutex.lock();
  m_finished = false;
  m_mutex.unlock();
  QThread::start();
}

void EventWriter::readingFinished()
{
  m_mutex.lock();
  m_finished = true;
  m_mutex.unlock();
  wait();
}

void EventWriter::run()
{
  // continuously write events until reading is finished
  while(true) {
    writeEvent(m_queue->dequeue());

    m_mutex.lock();
    if (m_finished){
      m_mutex.unlock();
      break;
    }
    m_mutex.unlock();
  }

  // write the events still left in the queue
  while(m_queue->numberOfEvents() > 0) {
    writeEvent(m_queue->dequeue());
  }

  m_tree->GetUserInfo()->Add(m_description);
  m_file->cd();
  m_tree->Write();
  m_file->Close();
}

void EventWriter::processEvent(const AnalyzedEvent* event)
{
  m_queue->enqueue(const_cast<SimpleEvent*>(event->simpleEvent()));
}

void EventWriter::writeEvent(SimpleEvent* event)
{
  if (event) {
    DataChain::s_mutex.lock();
    m_event = event;
    m_tree->Fill();
    DataChain::s_mutex.unlock();
    delete m_event;
  }
}
