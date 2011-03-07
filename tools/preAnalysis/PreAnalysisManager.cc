#include "PreAnalysisManager.hh"

#include "ClusterGenerator.hh"
#include "EventReader.hh"
#include "EventWriter.hh"

#include <iostream>

#include <QTimer>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>

PreAnalysisManager::PreAnalysisManager(QObject* parent) :
  QObject(parent),
  m_timer(new QTimer),
  m_reader(new EventReader),
  m_writer(new EventWriter),
  m_generators()
{
  m_timer->setInterval(500);
  connect(m_reader, SIGNAL(started()), m_timer, SLOT(start()));
  connect(m_writer, SIGNAL(finished()), m_timer, SLOT(stop()));
  connect(m_timer, SIGNAL(timeout()), this, SLOT(updateStatus()));

  connect(m_reader, SIGNAL(finished()), m_writer, SLOT(readingFinished()));
  connect(m_writer, SIGNAL(finished()), this, SLOT(finish()));
}

PreAnalysisManager::~PreAnalysisManager()
{
  delete m_timer;
  delete m_reader;
  delete m_writer;
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
      sourceFileNames.append(line);
      line = inStream.readLine();
    }
    if (argument.endsWith(".txt"))
      argument.replace(QRegExp("\\.txt$"), "");
    destinationFileName = argument + ".root";
  }

  m_writer->init(destinationFileName);

  for (int i = 0; i < sourceFileNames.size(); i++) {
    QString fileName = sourceFileNames.at(i);
    m_reader->addRootFile(fileName);
    m_writer->addInputFileToDescription(fileName);
  }
}

void PreAnalysisManager::start(int nThreads)
{
  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;

  for (int i = 0; i < nThreads; i++) {
    ClusterGenerator* clusterGen = new ClusterGenerator;
    m_generators.append(clusterGen);
    clusterGen->addDestination(m_writer);
  }
  m_reader->setBufferSize(100);
  m_reader->start(m_generators);
  m_writer->start();
}

void PreAnalysisManager::updateStatus()
{
  static int prevValue = 0;
  int currentValue = m_reader->progress();

  int toDraw = currentValue - prevValue;
  for (int i = 0; i < toDraw; i++) {
    std::cout << "#" << std::flush;
  }

  prevValue = currentValue;
}

void PreAnalysisManager::finish()
{
  updateStatus();
  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "Finished data reduction!" << std::endl;
  qApp->quit();
}
