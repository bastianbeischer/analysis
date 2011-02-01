#ifndef MCSINGLEFILE_HH
#define MCSINGLEFILE_HH

#include <QList>
//#include <QMap>

#include <QDebug>

class QString;
class MCEvent;
class RunFile;

class MCSingleFile
{
public:
  MCSingleFile();
  MCSingleFile(QString fileName);
  ~MCSingleFile();

public:
  unsigned int    getNumberOfEvents()                                    const {return m_MCEvents.size();}
  const MCEvent*  getMCEvent(unsigned int eventNo)                       const {qDebug() << getNumberOfEvents();return m_MCEvents.at(eventNo);}
  bool            IsGood();

private:
  void init();
  void open(QString fileName);
  void cleanupLists();

private:
  RunFile*                      m_runFile;

private:
  QList<const MCEvent*>         m_MCEvents;

};

#endif // MCSINGLEFILE_HH
