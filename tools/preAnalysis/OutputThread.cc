#include "OutputThread.hh"

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

OutputThread::OutputThread(QObject* parent) :
  QThread(parent),
  EventDestination(),
  m_mutex(),
  m_finished(true),
  m_queue(new EventQueue)
{
}

OutputThread::~OutputThread()
{
  delete m_file;
  delete m_queue;
}

void OutputThread::init(QString filename)
{
  m_file = new TFile(qPrintable(filename), "RECREATE");
  m_tree = new TTree("SimpleEventTree", "tree with simple events");
  m_tree->SetDirectory(m_file);
  m_event = 0;
  m_tree->Branch("event", "SimpleEvent", &m_event);

  m_description = new DataDescription;
  m_description->setSoftwareVersionHash();
}

void OutputThread::start()
{
  m_mutex.lock();
  m_finished = false;
  m_mutex.unlock();
  QThread::start();
}

void OutputThread::readingFinished()
{
  m_mutex.lock();
  m_finished = true;
  m_mutex.unlock();
  wait();
}

void OutputThread::run()
{
  // continuously write events until reading is finished
  while(!m_finished) {
    writeEvent(m_queue->dequeue());
  } 

  // write the events still left in the queue
  while(m_queue->numberOfEvents() > 0) {
    std::cout << m_queue->numberOfEvents() << std::endl;
    writeEvent(m_queue->dequeue());
  }

  m_tree->GetUserInfo()->Add(m_description);
  m_file->cd();
  m_tree->Write();
  m_file->Close();
}

void OutputThread::processEvent(const QVector<Hit*>&, Track*, SimpleEvent* event)
{
  m_queue->enqueue(event);
}

void OutputThread::writeEvent(SimpleEvent* event)
{
  if (event) {
    DataChain::s_mutex.lock();
    m_event = event;
    m_tree->Fill();
    DataChain::s_mutex.unlock();
    delete m_event;
  }
}
