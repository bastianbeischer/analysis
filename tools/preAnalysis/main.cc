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
	
  TFile destinationTreeFile(qPrintable(listName+".root"), "RECREATE");
  TTree destinationTree("SimpleEventTree", "tree with simple events");
  SimpleEvent* destinationEvent = 0;
  destinationTree.Branch("event", "SimpleEvent", &destinationEvent);
  DataDescription description;
  description.calculateSoftwareVersionHash();

  // QFile runFileListFile(listName);
  // if (!runFileListFile.open(QIODevice::ReadOnly | QIODevice::Text))
  //   qFatal("Could not open run file list file.");
    // description.addRunFile(line.toStdString());

  DataChain chain(qPrintable(listName));
  unsigned int nEntries = chain.nEntries();

  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;
  int iFactors = 0;

  Setup* setup = Setup::instance();

  for (unsigned int i = 0; i < nEntries; i++) {
    SimpleEvent* sourceEvent = chain.event(i);
    destinationEvent = new SimpleEvent();

    // do the zero compression
    // vector of all hits in this event
    QVector<Hit*> hits;
    foreach(Hit* hit, sourceEvent->hits())
      hits.push_back(hit);

    // add hits to the detectors
    foreach(Hit* hit, hits) {
      if (hit->type() == Hit::tracker || hit->type() == Hit::trd) {
        double z = hit->position().z();
        Layer* layer = setup->layer(z);
        layer->addHitToDetector(hit);
      }
    }

    // find clusters (currently TRD and Tracker)
    Layer* layer = setup->firstLayer();
    while(layer) {
    
      // QVector<Cluster*> clustersHere = layer->clusters();
      // foreach(Cluster* cluster, clustersHere)
      //   clusters.push_back(cluster);

      Cluster* cluster = layer->bestCluster();
      if (cluster)
        destinationEvent->addHit(cluster);
      layer->clearHitsInDetectors();

      // update pointer
      layer = setup->nextLayer();
    }

    destinationTree.Fill();

    if ( i > iFactors*nEntries/100. ) {
      std::cout << "#" << std::flush;
      iFactors++;
    }

  } // loop over entries

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;

  destinationTree.GetUserInfo()->Add(&description);
  destinationTreeFile.cd();
  destinationTree.Write();
  destinationTreeFile.Close();

  return EXIT_SUCCESS;
}
