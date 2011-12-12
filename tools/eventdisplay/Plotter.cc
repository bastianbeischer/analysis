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
#include "Reconstruction.hh"
#include "TimeOfFlight.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "Hypothesis.hh"

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

void Plotter::drawEvent(unsigned int i, Enums::TrackType type, bool allClusters, QTextEdit& infoTextEdit,
  TQtWidget& trackFindingWidget, ReconstructionMethodGraph& widget1, ReconstructionMethodGraph& widget2)
{
  TCanvas* canvas = GetCanvas();
  canvas->cd();

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

  const Reconstruction* reconstruction = 0;
  reconstruction = m_processor->reconstruction(widget1.method());
  Q_ASSERT(reconstruction);
  widget1.draw(reconstruction->graph(), reconstruction->legend());
  reconstruction = m_processor->reconstruction(widget2.method());
  Q_ASSERT(reconstruction);
  widget2.draw(reconstruction->graph(), reconstruction->legend());

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
  infoTextEdit.append(QString("time of event: %1").arg(timeOfEvent.toString("dd.MM.yyyy hh:mm:ss.zzz")));
  infoTextEdit.append(QString("\nroot file: %1").arg(rootFileName));
  infoTextEdit.append(QString("event in root file: %1").arg(eventNumberInRootFile));
  infoTextEdit.append(QString("\nrunfile: %1").arg(runFileNo));
  infoTextEdit.append(QString("run start: %1").arg(timeOfRun.toString("dd.MM.yyyy hh:mm:ss.zzz")));
  infoTextEdit.append(QString("runfilename: %1").arg(runfileName));
  infoTextEdit.append(QString("event in runfile: %1").arg(eventInRunFile));
  if (event->contentType() == SimpleEvent::MonteCarlo) {
    const MCEventInformation* mcInfo = event->MCInformation();
    const MCSimpleEventParticle* mcPrimary = mcInfo->primary();
    infoTextEdit.append("\nMonte-Carlo Information:");
    infoTextEdit.append("PDG ID =\t" + QString::number(mcPrimary->pdgID));
    infoTextEdit.append("Particle Name =\t" + ParticleDB::instance()->lookupPdgId(mcPrimary->pdgID)->name());
    infoTextEdit.append("momentum =\t" + QString::number(mcPrimary->initialMomentum.Mag(),'f',3) + "GeV");
  }

  const Particle* particle = m_processor->particle();
  if (particle->track()) {
    infoTextEdit.append(QString("\nSpectrometer: R = %1 GV").arg(particle->track()->rigidity(), 0, 'g', 3));
    infoTextEdit.append(QString("TOF: beta = %1").arg(particle->beta()));
  }

  if (SettingsManager::instance()->settingsForEvent(event)->situation() == Enums::Testbeam11)
    infoTextEdit.append(QString("Cherenkovs: %1, %2 ADC counts\n")
      .arg(event->sensorData(SensorTypes::BEAM_CHERENKOV1)).arg(event->sensorData(SensorTypes::BEAM_CHERENKOV2)));


  QString itemBegin = "<td valign=\"middle\" align=\"center\">";
  QString itemEnd = "</td>\n";

  QString reconstructionTable;
  reconstructionTable+= "<table border=\"1\">\n<tr>\n";
  reconstructionTable+= itemBegin + "R/GV<br>(logL)" + itemEnd;
  for (Enums::ParticleIterator particleIt = Enums::particleBegin(); particleIt != Enums::particleEnd(); ++particleIt)
    if (particleIt.key() & m_processor->particles())
      reconstructionTable+= itemBegin + particleIt.value() + itemEnd;
  reconstructionTable+= itemBegin + "sum" + itemEnd;
  reconstructionTable+= "</tr>\n";
  QString additionalReconstructionTable = reconstructionTable;

  Enums::ReconstructionMethodIterator reconstructionEnd = Enums::reconstructionMethodEnd();
  for (Enums::ReconstructionMethodIterator reconstructionIt = Enums::reconstructionMethodBegin(); reconstructionIt != reconstructionEnd; ++reconstructionIt) {
    //qDebug() << reconstructionIt.value();
    const Reconstruction* reconstruction = m_processor->reconstruction(reconstructionIt.key());
    QString& table = reconstruction->externalInformation() ? additionalReconstructionTable : reconstructionTable; 
    table+= "<tr>\n";
    table+= itemBegin + reconstructionIt.value().left(4) + itemEnd;
    QVector<Enums::Particle>::ConstIterator particleIt = reconstruction->particles().begin();
    QVector<Enums::Particle>::ConstIterator particleEnd = reconstruction->particles().end();
    QVector<QPointF>::ConstIterator minimumIt = reconstruction->minima().begin();
    for (int it = 0; particleIt != particleEnd; ++minimumIt, ++particleIt, ++it) {
      const Hypothesis* hypothesis = m_processor->particle()->hypothesis(*particleIt, reconstructionIt.key());
      QString text = QString("%1<br>%2").arg(hypothesis->rigidity(), 0, 'g', 2).arg(hypothesis->logLikelihood(), 0, 'g', 2);
      if (it == reconstruction->indexOfGlobalMinimum()) {
        text.prepend("<span style=\"color:red\">");
        text.append("</span>");
      }
      table+= itemBegin + text + itemEnd; 
    }
    table+= itemBegin + itemEnd;
    table+= "</tr>\n";
  }

  reconstructionTable+= "</table>";
  infoTextEdit.append("PERDaix only:");
  infoTextEdit.append(reconstructionTable);

  additionalReconstructionTable+= "</table>";
  infoTextEdit.append("\ntogether with external information:");
  infoTextEdit.append(additionalReconstructionTable);

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
