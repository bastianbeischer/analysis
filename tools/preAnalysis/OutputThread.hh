#ifndef OutputThread_hh
#define OutputThread_hh

#include <QThread>
#include "EventDestination.hh"

#include <QMutex>
#include <QString>

class EventQueue;
class TFile;
class TTree;
class Track;
class SimpleEvent;
class DataDescription;

class OutputThread :
  public QThread,
  public EventDestination
{
  
Q_OBJECT

public:
  OutputThread(QObject* = 0);
  ~OutputThread();
  
  void init(QString filename);
  DataDescription* description() {return m_description;}
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);

  void start();

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

#endif /* OutputThread_hh */
