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
  MCSingleFile(const QString& fileName);
  ~MCSingleFile();

public:
  unsigned int getNumberOfEvents() const;
  const MCEvent* getNextMCEvent() const;
  bool isGood();

private:
  void open(const QString& fileName);

private:
  RunFile* m_runFile;

};

#endif // MCSINGLEFILE_HH
