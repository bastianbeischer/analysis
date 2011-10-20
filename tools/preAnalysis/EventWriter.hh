#ifndef EventWriter_hh
#define EventWriter_hh

#include <QThread>
#include "EventDestination.hh"

#include <QMutex>
#include <QString>

class EventQueue;
class TFile;
class TTree;
class Particle;
class SimpleEvent;
class DataDescription;

class EventWriter :
  public QThread,
  public EventDestination
{
  
Q_OBJECT

public:
  EventWriter(QObject* = 0);
  ~EventWriter();
  
  void init(QString filename);
  void processEvent(const AnalyzedEvent*);
  void start();

  void addInputFileToDescription(QString filename);

public slots:
  void readingFinished();

protected:
  void run();
  void writeEvent(SimpleEvent*);

private:
  QMutex m_mutex;
  bool m_finished;
  TFile* m_file;
  TTree* m_tree;
  SimpleEvent* m_event;
  DataDescription* m_description;
  EventQueue* m_queue;
  
};

#endif /* EventWriter_hh */
