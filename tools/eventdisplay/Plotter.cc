#include "Plotter.hh"

#include "DataChain.hh"
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
#include "AnalysisProcessor.hh"
#include "ParticleProperties.hh"
#include "ParticleDB.hh"
#include "LikelihoodAnalysis.hh"

#include <TCanvas.h>
#include <TH2I.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>
#include <TPad.h>
#include <TMultiGraph.h>

#include <QPlainTextEdit>
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
  , m_track(0)
  , m_processor(new AnalysisProcessor)
  , m_hitsPlot(new HitsPlot)
  , m_positionLabel(0)
{
  m_processor->setCorrectionFlags(~Enums::Corrections(0));
  m_processor->setParticleFilter(~Enums::Particles(0));
  m_processor->addDestination(m_hitsPlot);
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

void Plotter::addRootFile(const QString& file)
{
  m_chain->addRootFile(qPrintable(file));
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

void Plotter::drawEvent(unsigned int i, Enums::TrackType type, bool allClusters,
  QPlainTextEdit& infoTextEdit, TQtWidget& trackFindingWidget, TQtWidget& likelihoodWidget)
{
  TCanvas* canvas = GetCanvas();
  canvas->cd();

  if (m_track) {
    delete m_track;
    m_track = 0;
  }
  Q_ASSERT(i < numberOfEvents());
  m_hitsPlot->setDrawAllClusters(allClusters);
  SimpleEvent* event = m_chain->event(i);
  m_processor->setTrackType(type);
  m_processor->process(event);

  canvas->Modified();
  canvas->Update();

  TCanvas* tfCan = trackFindingWidget.GetCanvas();
  tfCan->cd();
  tfCan->Clear();
  m_processor->trackFinding()->trackFindingHist()->Draw("colz");
  tfCan->Modified();
  tfCan->Update();
  TCanvas* lhCanvas = likelihoodWidget.GetCanvas();
  lhCanvas->cd();
  lhCanvas->Clear();
  const LikelihoodAnalysis* lh = m_processor->likelihood();
  TMultiGraph* mg = lh->graph();
  mg->Draw("ALP");
  mg->GetYaxis()->SetRangeUser(-10., 100.);
  lhCanvas->Modified();
  lhCanvas->Update();

  //show info for event
  const DataDescription* currentDesc = m_chain->currentDescription();
  quint32 eventNumberInRootFile = m_chain->entryInFile();
  quint32 runFileNo = currentDesc->runFileForEventNumber(eventNumberInRootFile);
  quint32 eventInRunFile = currentDesc->eventNumberInRunFile(eventNumberInRootFile);
  QString rootFileName = QString::fromStdString(m_chain->currentFile()->GetName());
  QString runfileName = QString::fromStdString(currentDesc->runFileNameForEventNumber(eventNumberInRootFile));
  QDateTime timeOfRun = QDateTime::fromTime_t(event->runStartTime());
  QDateTime timeOfEvent = timeOfRun.addMSecs(event->eventTime());

  infoTextEdit.clear();
  infoTextEdit.appendPlainText(QString("time of event: %1").arg(timeOfEvent.toString("dd.MM.yyyy hh:mm:ss.zzz")));
  infoTextEdit.appendPlainText(QString("\nroot file: %1").arg(rootFileName));
  infoTextEdit.appendPlainText(QString("event in root file: %1").arg(eventNumberInRootFile));
  infoTextEdit.appendPlainText(QString("\nrunfile: %1").arg(runFileNo));
  infoTextEdit.appendPlainText(QString("run start: %1").arg(timeOfRun.toString("dd.MM.yyyy hh:mm:ss.zzz")));
  infoTextEdit.appendPlainText(QString("runfilename: %1").arg(runfileName));
  infoTextEdit.appendPlainText(QString("event in runfile: %1").arg(eventInRunFile));
  if (event->contentType() == SimpleEvent::MonteCarlo) {
    const MCEventInformation* mcInfo = event->MCInformation();
    const MCSimpleEventParticle* mcPrimary = mcInfo->primary();
    infoTextEdit.appendPlainText("\nMonte-Carlo Information:");
    infoTextEdit.appendPlainText("PDG ID =\t" + QString::number(mcPrimary->pdgID));
    infoTextEdit.appendPlainText("Particle Name =\t" + ParticleDB::instance()->lookupPdgId(mcPrimary->pdgID)->name());
    infoTextEdit.appendPlainText("momentum =\t" + QString::number(mcPrimary->initialMomentum.Mag(),'f',3) + "GeV");
  }
  infoTextEdit.appendPlainText("\nLikelihood:");
  QVector<Enums::Particle>::ConstIterator particleIt = lh->particles().begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = lh->particles().end();
  QVector<QPointF>::ConstIterator minimumIt = lh->minima().begin();
  for (int it = 0; particleIt != particleEnd; ++it, ++particleIt, ++minimumIt) {
    QString text = QString("%1.) (%2GV, %3) %4").arg(it + 1).arg(minimumIt->x(), 0, 'g', 3)
      .arg(minimumIt->y(), 0, 'g', 3).arg(Enums::label(*particleIt));
    if (it == lh->indexOfGlobalMinimum()) {
      text.prepend("<span style=\" color:#ff0000;\">");
      text.append("</span>");
      infoTextEdit.appendHtml(text);
    } else {
      infoTextEdit.appendPlainText(text);
    }
  }
  delete event;
}

void Plotter::saveCanvas(const QString& fileName)
{
  GetCanvas()->SaveAs(qPrintable(fileName));
}

void Plotter::unzoom()
{
}

void Plotter::updateTrackFindingParameters(int slopeBins, int offsetBins, double trackerPull, double trdPull)
{
  m_trackFinding->setSlopeBins(slopeBins);
  m_trackFinding->setOffsetBins(offsetBins);
  m_trackFinding->setTrackerPull(trackerPull);
  m_trackFinding->setTrdPull(trdPull);
}
