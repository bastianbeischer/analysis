#include "DataChain.hh"

#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Track.hh"
#include "Layer.hh"
#include "TrackFinding.hh"
#include "ResidualPlot.hh"
#include "Setup.hh"
#include "DataDescription.hh"

#include <TCanvas.h>
#include <TH2I.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>

#include <QVector>

#include <fstream>
#include <iostream>

DataChain::DataChain() :
  m_chain(new TChain("SimpleEventTree")),
  m_event(0),
  m_trackFinding(new TrackFinding)
{
  m_chain->SetBranchAddress("event", &m_event);
}

DataChain::~DataChain()
{
  delete m_chain;
  delete m_event;
  foreach(Track* track, m_tracks)
    delete track;
  delete m_trackFinding;
}

void DataChain::addFiles(const char* listName)
{
  ifstream file(listName);
  if (!file.is_open()) {
    std::cout << "Error opening file" << std::endl;
  }
  while (true) {
    // read filename from list
    char filename[256];
    file >> filename;
    if (file.eof()) break;

    // starting output
    std::cout << "Adding " <<  filename;

    // version string
    TFile file(filename, "READ");
    TTree* tree = (TTree*)file.Get("SimpleEventTree");
    DataDescription* desc = (DataDescription*) tree->GetUserInfo()->First();
    if (desc) 
      std::cout << " (version: " << desc->softwareVersionHash() << ")";
    
    // complete output
    std::cout << std::endl;

    // actually add the file
    m_chain->AddFile(filename);
  }
  
  std::cout << "DONE: Chain contains " << m_chain->GetEntries() << " events" << std::endl;
}

void DataChain::process()
{
  unsigned int nEntries = m_chain->GetEntries();

  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;
  int iFactors = 0;

  Setup* setup = Setup::instance();

  for (unsigned int entry = 0; entry < nEntries; entry++) {
    m_chain->GetEntry(entry);

    // vector of all hits in this event
    QVector<Hit*> hits;
    foreach(Hit* hit, m_event->hits())
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
    QVector<Hit*> clusters;
    Layer* layer = setup->firstLayer();
    while(layer) {
    
      // QVector<Cluster*> clustersHere = layer->clusters();
      // foreach(Cluster* cluster, clustersHere)
      //   clusters.push_back(cluster);

      Cluster* cluster = layer->bestCluster();
      if (cluster)
        clusters.push_back(cluster);
      layer->clearHitsInDetectors();

      // update pointer
      layer = setup->nextLayer();
    }

    // track finding
    clusters = m_trackFinding->findTrack(clusters);

    // fit once for each layer
    layer = setup->firstLayer();
    while(layer) {
      double z = layer->z();

      // create track for this layer
      if(!m_tracks[layer])
        m_tracks[layer] = new Track;

      // remove clusters in this layer from clusters for track fit
      QVector<Hit*> clustersForFit;
      QVector<Hit*> clustersInThisLayer;
      
      foreach(Hit* hit, clusters) {
        if (hit->position().z() != z)
          clustersForFit.push_back(hit);
        else
          clustersInThisLayer.push_back(hit);
      }

      if (!m_residualPlots[layer])
        m_residualPlots[layer] = new ResidualPlot(z);

      // fit and fill histograms
      if (m_tracks[layer]->fit(clustersForFit))
        foreach(Hit* hit, clustersInThisLayer)
          m_residualPlots[layer]->fill(hit, m_tracks[layer]);

      layer = setup->nextLayer();
    }

    // clusters were dynamically allocated and have to be delete by hand. only clusters though. hacky here. also: clusters not on track won't be deleted.
    // needs a real solution soon.
    for (QVector<Hit*>::iterator it = clusters.begin() ; it != clusters.end(); it++) {
      Cluster* cluster = dynamic_cast<Cluster*>(*it);
      if (cluster)
        delete cluster;
    }

    if ( entry > iFactors*nEntries/100. ) {
      std::cout << "#" << std::flush;
      iFactors++;
    }

  } // loop over entries

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;

}

void DataChain::draw()
{
  foreach(ResidualPlot* plot, m_residualPlots)
    plot->draw();
}
