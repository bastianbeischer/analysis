#include "PreAnalysisManager.hh"

#include "ClusterGenerator.hh"
#include "EventReader.hh"
#include "EventWriter.hh"

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>

PreAnalysisManager::PreAnalysisManager(QObject* parent) :
  QObject(parent),
  m_reader(new EventReader),
  m_outputThread(new EventWriter),
  m_generators()
{
  connect(m_reader, SIGNAL(finished()), m_outputThread, SLOT(readingFinished()));
  connect(m_outputThread, SIGNAL(finished()), qApp, SLOT(quit()));
}

PreAnalysisManager::~PreAnalysisManager()
{
  delete m_reader;
  delete m_outputThread;
  qDeleteAll(m_generators);
}

void PreAnalysisManager::processArgument(QString argument)
{
  // loop over all list of files
  QStringList sourceFileNames;
  QString destinationFileName;

  if (argument.endsWith(".root")) {
    sourceFileNames.append(argument);
    destinationFileName = argument.replace(QRegExp("\\.root$"), "_clusters.root");
  }
  else {
    QFile runFileListFile(argument);
    if (!runFileListFile.open(QIODevice::ReadOnly | QIODevice::Text))
      qFatal("Could not open run file list file.");

    QTextStream inStream(&runFileListFile);
    QString line = inStream.readLine();
    while (!line.isNull()) {
      qDebug() << line;
      sourceFileNames.append(line);
      line = inStream.readLine();
    }
    if (argument.endsWith(".txt"))
      argument.replace(QRegExp("\\.txt$"), "");
    destinationFileName = argument + ".root";
  }

  m_outputThread->init(destinationFileName);

  for (int i = 0; i < sourceFileNames.size(); i++) {
    QString fileName = sourceFileNames.at(i);
    m_reader->addRootFile(fileName);
  }
}

void PreAnalysisManager::start(int nThreads)
{
  for (int i = 0; i < nThreads; i++) {
    ClusterGenerator* clusterGen = new ClusterGenerator;
    m_generators.append(clusterGen);
    clusterGen->addDestination(m_outputThread);
  }
  m_reader->start(m_generators);
  m_outputThread->start();
}
