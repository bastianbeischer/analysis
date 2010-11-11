#include "Plotter.hh"

#include "DataChain.hh"
#include "Cluster.hh"
#include "TOFCluster.hh"
#include "SimpleEvent.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
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

Plotter::Plotter() :
  m_chain(new DataChain),
  m_trackFinding(new TrackFinding)
{
}

Plotter::~Plotter()
{
  delete m_chain;
  foreach(Track* track, m_tracks)
    delete track;
  delete m_trackFinding;
}

void Plotter::addFiles(const char* listName)
{
  m_chain->addFileList(listName);
}

void Plotter::process()
{
  unsigned int nEntries = m_chain->nEntries();

  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;
  int iFactors = 0;

  Setup* setup = Setup::instance();

  for (unsigned int i = 0; i < nEntries; i++) {
    SimpleEvent* event = m_chain->event(i);

    // vector of all hits in this event
    QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
    QVector<Hit*> clusters;
    foreach(Cluster* cluster, setup->generateClusters(hits))
      clusters.push_back(cluster);

    // track finding
    clusters = m_trackFinding->findTrack(clusters);

    // fit once for each layer
    Layer* layer = setup->firstLayer();
    while(layer) {
      double z = layer->z();

      // create track for this layer
      if(!m_tracks[layer])
        m_tracks[layer] = new BrokenLine;

      // remove clusters in this layer from clusters for track fit
      QVector<Hit*> clustersForFit;
      QVector<Hit*> clustersInThisLayer;
      
      foreach(Hit* hit, clusters) {
        if (hit->position().z() != z) {
          clustersForFit.push_back(hit);
        }
        else {
          clustersInThisLayer.push_back(hit);
        }
      }

      if (!m_residualPlots[layer])
        m_residualPlots[layer] = new ResidualPlot(z);

      // fit and fill histograms
      if (m_tracks[layer]->fit(clustersForFit))
        foreach(Hit* hit, clustersInThisLayer)
          m_residualPlots[layer]->fill(hit, m_tracks[layer]);

      layer = setup->nextLayer();
    }

    // delete clusters in case they were allocated dynamically
    setup->deleteClusters();

    if ( i > iFactors*nEntries/100. ) {
      std::cout << "#" << std::flush;
      iFactors++;
    }

  } // loop over entries

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;

}

void Plotter::draw()
{
  foreach(ResidualPlot* plot, m_residualPlots)
    if (plot->z() > -240 && plot->z() < 240)
      plot->draw();
}
