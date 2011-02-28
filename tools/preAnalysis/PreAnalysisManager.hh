#ifndef PreAnalysisManager_hh
#define PreAnalysisManager_hh

#include <QObject>

#include <QVector>

class EventReader;
class EventProcessor;
class OutputThread;

class PreAnalysisManager :
  public QObject
{

Q_OBJECT

public:
  PreAnalysisManager(QObject* = 0);
  ~PreAnalysisManager();
  
  void processArgument(QString);
  void start(int);

private:
  EventReader* m_reader;
  OutputThread* m_outputThread;
  QVector<EventProcessor*> m_generators;

};

#endif /* PreAnalysisManager_hh */
