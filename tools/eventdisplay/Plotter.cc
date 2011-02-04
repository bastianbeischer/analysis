#include "Plotter.hh"

#include "DataChain.hh"
#include "Cluster.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "SimpleEvent.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"
#include "StraightLine.hh"
#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Corrections.hh"
#include "DataDescription.hh"
#include "HitsPlot.hh"

#include <TCanvas.h>
#include <TH2I.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>
#include <TPad.h>

#include <QTextBrowser>
#include <QLabel>
#include <QVector>
#include <QDateTime>
#include <QDebug>

#include <fstream>
#include <iostream>

Plotter::Plotter(QWidget* parent)
  : TQtWidget(parent)
  , m_chain(new DataChain)
  , m_trackFinding(new TrackFinding)
  , m_corrections(new Corrections)
  , m_track(0)
  , m_hitsPlot(new HitsPlot)
  , m_positionLabel(0)
{
  m_hitsPlot->draw(GetCanvas());
  gPad->Update();
  setMouseTracking(true);
}

Plotter::~Plotter()
{
  if (m_track)
    delete m_track;
  delete m_chain;
  delete m_trackFinding;
  delete m_corrections;
  delete m_hitsPlot;
}

void Plotter::setFileList(const QString& listName)
{
  m_chain->setFileList(qPrintable(listName));
}

void Plotter::addFileList(const QString& listName)
{
  m_chain->addFileList(qPrintable(listName));
}

unsigned int Plotter::numberOfEvents()
{
  return m_chain->nEntries();
}

void Plotter::setPositionLabel(QLabel* label)
{
  m_positionLabel = label;
}

void Plotter::mouseMoveEvent(QMouseEvent* event)
{
  if (m_positionLabel) {
    double x = gPad->AbsPixeltoX(event->x());
    double z = gPad->AbsPixeltoY(event->y());
    double y = x / m_hitsPlot->yStretchFactor();
    m_positionLabel->setText(QString("x=%1%2 y=%3%4 z=%5%6")
      .arg(x < 0 ? "-" : "+").arg(qAbs(x), 6, 'f', 2, '0')
      .arg(y < 0 ? "-" : "+").arg(qAbs(y), 6, 'f', 2, '0')
      .arg(z < 0 ? "-" : "+").arg(qAbs(z), 6, 'f', 2, '0'));
  }
}

void Plotter::drawEvent(unsigned int i, bool drawTrack, int fitMethod, QTextBrowser& infoTextBrowser)
{
  if (m_track) {
    delete m_track;
    m_track = 0;
  }
  Q_ASSERT(i < numberOfEvents());
  SimpleEvent* event = m_chain->event(i);

  QVector<Hit*> clusters;
  QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
  if (event->contentType() == SimpleEvent::Clusters)
    clusters = hits;
  else
    clusters = Setup::instance()->generateClusters(hits);

  m_corrections->preFitCorrections(clusters);

  Track* track = 0;
  if (drawTrack) {
    // track finding
    clusters = m_trackFinding->findTrack(clusters);
    if (fitMethod == 0)
      track = new StraightLine;
    else if (fitMethod == 1)
      track = new BrokenLine;
    else if (fitMethod == 2)
      track = new CenteredBrokenLine;
    track->process(clusters);
  }
  m_hitsPlot->drawEvent(GetCanvas(), clusters, track);

  //show info for event
  const DataDescription* currentDesc = m_chain->currentDescription();
  quint32 eventNumberInRootFile = m_chain->entryInFile();
  quint32 runFileNo = currentDesc->runFileForEventNumber(eventNumberInRootFile);
  quint32 eventInRunFile = currentDesc->eventNumberInRunFile(eventNumberInRootFile);
  QString rootFileName = QString::fromStdString(m_chain->currentFile()->GetName());
  QString runfileName = QString::fromStdString(currentDesc->runFileNameForEventNumber(eventNumberInRootFile));
  QDateTime timeOfRun = QDateTime::fromTime_t(event->runStartTime());
  QDateTime timeOfEvent = timeOfRun.addMSecs(event->eventTime());

  infoTextBrowser.clear();
  infoTextBrowser.append("time of event:\n" + timeOfEvent.toString("dd.MM.yyyy hh:mm:ss.zzz"));
  infoTextBrowser.append("\n root file:\n " +  rootFileName);
  infoTextBrowser.append("\n event in root file:\n " +  QString::number(eventNumberInRootFile));
  infoTextBrowser.append("\n  runfile:\n  " +  QString::number(runFileNo));
  infoTextBrowser.append("\n  run start:\n  " +  timeOfRun.toString("dd.MM.yyyy hh:mm:ss.zzz"));
  infoTextBrowser.append("\n  runfilename:\n  " +  runfileName);
  infoTextBrowser.append("\n  event in runfile:\n  " +  QString::number(eventInRunFile));
  //infoTextBrowser.append("\n  ms in runfile:\n  " + QString::number(msOfEventInRun));

  if (event->contentType() == SimpleEvent::RawData)
    qDeleteAll(clusters);
}

void Plotter::saveCanvas(const QString& fileName)
{
  GetCanvas()->SaveAs(qPrintable(fileName));
}

void Plotter::unzoom()
{
  qDebug() << "Plotter::unzoom()";
}

void Plotter::updateTrackFindingParameters(int slopeBins, int offsetBins, double trackerPull, double trdPull)
{
  m_trackFinding->setSlopeBins(slopeBins);
  m_trackFinding->setOffsetBins(offsetBins);
  m_trackFinding->setTrackerPull(trackerPull);
  m_trackFinding->setTrdPull(trdPull);
}
