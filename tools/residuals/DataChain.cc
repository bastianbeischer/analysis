#include "DataChain.hh"

#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Track.hh"
#include "Layer.hh"
#include "TrackFinding.hh"
#include "ResidualPlot.hh"
#include "Setup.hh"

#include <TCanvas.h>
#include <TH2I.h>
#include <TChain.h>

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
  for (std::map<Layer*, Track*>::iterator it = m_tracks.begin(); it != m_tracks.end(); it++)
    delete it->second;
  delete m_trackFinding;
}

void DataChain::addFiles(const char* listName)
{
  ifstream file(listName);
  if (!file.is_open()) {
    std::cout << "Error opening file" << std::endl;
  }
  while (true) {
    char filename[256];
    file >> filename;
    if (file.eof()) break;
    std::cout << "Adding " <<  filename << std::endl;
    m_chain->AddFile(filename);
  }

  std::cout << "DONE: Chain contains " << m_chain->GetEntries() << " events" << std::endl;
}

std::vector<Hit*> DataChain::applyCuts(std::vector<Hit*> hits) const
{
  std::vector<Hit*> passedHits;

  const double cutTracker = 250;
  const double cutTrd = 10;
  const double cutTof = 170;

  for (unsigned int i = 0; i < hits.size(); i++) {
    Hit* hit = hits.at(i);
    if ( (hit->type() == Hit::trd     && hit->signalHeight() > cutTrd) ||
         (hit->type() == Hit::tracker && hit->signalHeight() > cutTracker) ||
         (hit->type() == Hit::tof     && hit->signalHeight() > cutTof)) {
      passedHits.push_back(hit);
    }
  }
  return passedHits;
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

  Setup* setup = Setup::instance();

  for (unsigned int entry = 0; entry < nEntries; entry++) {
    m_chain->GetEntry(entry);

    std::vector<Hit*> hits = m_event->hits();

    for (unsigned int i = 0; i < hits.size(); i++) {
      Hit* hit = hits.at(i);
      if (hit->type() == Hit::tracker || hit->type() == Hit::trd) {
        double z = hit->position().z();
        Layer* layer = setup->layer(z);
        layer->addHitToDetector(hit);
      }
    }

    // clusters
    std::vector<Hit*> clusters;
    Layer* layer = setup->firstLayer();
    while(layer) {
    
      // std::vector<Cluster*> clustersHere = layer->clusters();
      // for (std::vector<Cluster*>::iterator it = clustersHere.begin(); it != clustersHere.end(); it++) {
      //   clusters.push_back(*it);
      // }

      Cluster* cluster = layer->bestCluster();
      if (cluster)
        clusters.push_back(cluster);

      layer->clearHitsInDetectors();

      layer = setup->nextLayer();
    }

    // // trd hits
    // hits = applyCuts(hits);
    // for (std::vector<Hit*>::iterator it = hits.begin(); it != hits.end(); it++) {
    //   Hit* hit = *it;
    //   if (hit->type() == Hit::trd) {
    //     clusters.push_back(hit);
    //   }
    // }

    // track finding
    clusters = m_trackFinding->findTrack(clusters);

    // fit once for each layer
    layer = setup->firstLayer();
    while(layer) {
      double z = layer->z();

      // create track for this layer
      if(!m_tracks[layer])
        m_tracks[layer] = new Track;

      // remove this layer from clusters;
      std::vector<Hit*> clustersForFit;
      std::vector<Hit*> clustersInThisLayer;
      for (std::vector<Hit*>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        Hit* hit = *it;
        if (hit->position().z() != z)
          clustersForFit.push_back(hit);
        else
          clustersInThisLayer.push_back(hit);
      }

      if (!m_residualPlots[z])
        m_residualPlots[z] = new ResidualPlot(z);

      // fit and fill histograms
      if (m_tracks[layer]->fit(clustersForFit)) {
        for (std::vector<Hit*>::iterator it = clustersInThisLayer.begin(); it != clustersInThisLayer.end(); it++) {
          Hit* hit = *it;
          m_residualPlots[z]->fill(hit, m_tracks[layer]);
        }
      }

      layer = setup->nextLayer();
    }

    // clusters were dynamically allocated and have to be delete by hand. only clusters though. hacky here.
    for (std::vector<Hit*>::iterator it = clusters.begin() ; it != clusters.end(); it++) {
      Cluster* cluster = dynamic_cast<Cluster*>(*it);
      if (cluster)
        delete cluster;
    }

    if ( entry > 0 && entry < nEntries && entry % (nEntries/100) == 0  ) {
      std::cout << "#" << std::flush;
    }

  } // loop over entries

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;

}

void DataChain::draw()
{
  for (std::map<double, ResidualPlot*>::iterator it = m_residualPlots.begin(); it != m_residualPlots.end(); it++)
    it->second->draw();
}
