#include <cstdlib>
#include <iostream>

#include <QtGlobal>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

#include "DataChain.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "SimpleEvent.hh"
#include "DataDescription.hh"
#include "Hit.hh"
#include "Cluster.hh"

int main(int argc, char** argv)
{
  if (argc != 2)
    qFatal("Pass a ROOT file or a list of run files as argument!");

  QString argument = argv[1];

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

  TFile destinationTreeFile(qPrintable(destinationFileName), "RECREATE");
  TTree destinationTree("SimpleEventTree", "tree with simple events");
  SimpleEvent* destinationEvent = 0;
  destinationTree.Branch("event", "SimpleEvent", &destinationEvent);
  DataDescription description;
  description.setSoftwareVersionHash();

  for (int i = 0; i < sourceFileNames.size(); i++) {
    QString fileName = sourceFileNames.at(i);
    gROOT->cd();
    TFile sourceTreeFile(qPrintable(fileName), "READ");
    if (sourceTreeFile.IsZombie()) {
      qDebug("Could not open run file.");
      continue;
    }
    TTree* sourceTree = static_cast<TTree*>(sourceTreeFile.Get("SimpleEventTree"));
    SimpleEvent* sourceEvent = 0;
    sourceTree->SetBranchAddress("event", &sourceEvent);
    unsigned int nEvents = sourceTree->GetEntries();
    const std::string& hash = static_cast<DataDescription*>(sourceTree->GetUserInfo()->At(0))->softwareVersionHash();
    description.addRunFile(fileName.toStdString(), hash, nEvents);

    std::cout << std::endl;
    std::cout << "Adding file " << fileName.toStdString() << " with hash " << hash << std::endl;
    std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
    std::cout << "| Processing:                                                                                        |" << std::endl;
    std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
    std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
    std::cout << "|" << std::flush;
    int iFactors = 0;

    // loop over all events in this root file
    for (unsigned long eventIt = 0; eventIt < nEvents; ++eventIt) {
      sourceTree->GetEntry(eventIt);

      destinationEvent = new SimpleEvent(sourceEvent->eventId(), sourceEvent->runStartTime(), sourceEvent->eventTime(), sourceEvent->contentType());
      if (sourceEvent->contentType() == SimpleEvent::MonteCarlo){
        MCEventInformation* info = new MCEventInformation(*sourceEvent->MCInformation());
        destinationEvent->setMCInformation(info);
      }

      // vector of all hits in this event
      QVector<Hit*> hits = QVector<Hit*>::fromStdVector(sourceEvent->hits());

      // TODO
      // // do the zero compression
      // foreach(Hit* cluster, Setup::instance()->generateClusters(hits))
      //   destinationEvent->addHit(cluster);
      
      // copy sensor  data
      for (unsigned int i = SensorTypes::START; i < SensorTypes::END; i++) {
        destinationEvent->setSensorData((SensorTypes::Type)i, sourceEvent->sensorData((SensorTypes::Type)i) );
      }

      destinationTree.Fill();
      delete destinationEvent;

      if ( eventIt > iFactors*nEvents/100. ) {
        std::cout << "#" << std::flush;
        iFactors++;
      }
    } // events in this root file

    std::cout << "|" << std::endl;
    std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
    std::cout << "Finished file " << fileName.toStdString() << std::endl;
  } // all root files

  destinationTree.GetUserInfo()->Add(&description);
  destinationTreeFile.cd();
  destinationTree.Write();
  destinationTreeFile.Close();

  return EXIT_SUCCESS;
}
