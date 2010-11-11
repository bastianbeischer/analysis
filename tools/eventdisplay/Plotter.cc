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
#include "DataDescription.hh"
#include "HitsPlot.hh"

#include <TCanvas.h>
#include <TH2I.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>
#include <TPad.h>

#include <QLabel>
#include <QVector>
#include <QDebug>

#include <fstream>
#include <iostream>

Plotter::Plotter(QWidget* parent)
  : TQtWidget(parent)
  , m_chain(new DataChain)
  , m_trackFinding(new TrackFinding)
  , m_track(0)
  , m_hitsPlot(new HitsPlot)
  , m_positionLabel(0)
{
  setMouseTracking(true);
}

Plotter::~Plotter()
{
  if (m_track)
    delete m_track;
  delete m_chain;
  delete m_trackFinding;
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

void Plotter::mousePressEvent(QMouseEvent*)
{
}

void Plotter::mouseReleaseEvent(QMouseEvent*)
{
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

void Plotter::drawEvent(unsigned int i, bool drawTrack)
{
  if (m_track) {
    delete m_track;
    m_track = 0;
  }
  Setup* setup = Setup::instance();
  setup->deleteClusters();
  Q_ASSERT(i < numberOfEvents());
  SimpleEvent* event = m_chain->event(i);
  // vector of all hits in this event
  QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
  QVector<Hit*> clusters;
  foreach(Cluster* cluster, setup->generateClusters(hits))
    clusters.push_back(cluster);
  if (drawTrack) {
    // track finding
    clusters = m_trackFinding->findTrack(clusters);
    m_track = new CenteredBrokenLine;
    if (m_track->fit(clusters)) {
      m_hitsPlot->draw(GetCanvas(), clusters, m_track);
      return;
    }
  }
  m_hitsPlot->draw(GetCanvas(), clusters);
}

void Plotter::saveCanvas(const QString& fileName)
{
  GetCanvas()->SaveAs(qPrintable(fileName));
}

void Plotter::unzoom()
{
  qDebug() << "Plotter::unzoom()";
}
