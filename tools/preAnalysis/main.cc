#include <cstdlib>
#include <iostream>

#include <QtGlobal>
#include <QFile>
#include <QTextStream>
#include <QDebug>

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
    qFatal("Pass a run file list file!");

  QString listName = argv[1];
  QFile runFileListFile(listName);
  if (!runFileListFile.open(QIODevice::ReadOnly | QIODevice::Text))
    qFatal("Could not open run file list file.");
	
  TFile destinationTreeFile(qPrintable(listName+".root"), "RECREATE");
  TTree destinationTree("SimpleEventTree", "tree with simple events");
  SimpleEvent* destinationEvent = 0;
  destinationTree.Branch("event", "SimpleEvent", &destinationEvent);
  DataDescription description;
  description.calculateSoftwareVersionHash();

  Setup* setup = Setup::instance();

  // loop over all root files
  QTextStream inStream(&runFileListFile);
  QString line = inStream.readLine();
  while (!line.isNull()) {
    qDebug() << line;
    gROOT->cd();
    TFile sourceTreeFile(qPrintable(line));
    if (sourceTreeFile.IsZombie()) {
      qDebug("Could not open run file.");
      continue;
    }
    TTree* sourceTree = static_cast<TTree*>(sourceTreeFile.Get("SimpleEventTree"));
    SimpleEvent* sourceEvent = 0;
    sourceTree->SetBranchAddress("event", &sourceEvent);
    unsigned int nEvents = sourceTree->GetEntries();
    description.addRunFile(line.toStdString(), nEvents);

    // loop over all events in this root file
    for (unsigned long eventIt = 0; eventIt < nEvents; ++eventIt) {
      sourceTree->GetEntry(eventIt);
      destinationEvent = new SimpleEvent;

      // vector of all hits in this event
      QVector<Hit*> hits = QVector<Hit*>::fromStdVector(sourceEvent->hits());

      // do the zero compression
      foreach(Cluster* cluster, setup->generateClusters(hits))
        destinationEvent->addHit(cluster);
      
      destinationTree.Fill();

    } // events in this root file

    line = inStream.readLine();
  } // all root files

  destinationTree.GetUserInfo()->Add(&description);
  destinationTreeFile.cd();
  destinationTree.Write();
  destinationTreeFile.Close();

  return EXIT_SUCCESS;
}
