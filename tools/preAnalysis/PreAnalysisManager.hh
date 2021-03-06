#ifndef PreAnalysisManager_hh
#define PreAnalysisManager_hh

#include <QObject>

#include <QVector>

class EventReader;
class EventProcessor;
class EventWriter;
class QTimer;

class PreAnalysisManager :
  public QObject
{

Q_OBJECT

public:
  PreAnalysisManager(QObject* = 0);
  ~PreAnalysisManager();
  
  void processArgument(QString);
  void start(int);

private slots:
  void updateStatus();
  void finish();

private:
  QTimer*      m_timer;
  EventReader* m_reader;
  EventWriter* m_writer;
  QVector<EventProcessor*> m_generators;

};

#endif /* PreAnalysisManager_hh */
