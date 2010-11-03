#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include "TH2D.h"

#include "DataChain.hh"
#include "Cluster.hh"
#include "Layer.hh"
#include "Setup.hh"
#include "TrackFinding.hh"
#include "SimpleEvent.hh"

#include <QVector>

#include <iostream>

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cout << "Please give a filename to open. Should be a list of root files. Example: residuals all_runs.txt" << std::endl;
    return -1;
  }
  
  const char* filename = argv[1];

  TApplication app("app", &argc, argv);
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1,0);
  gStyle->SetOptFit(111);

  DataChain dc;
  dc.addFiles(filename);

  unsigned int nEntries = dc.nEntries();

  std::cout << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;
  std::cout << "| Processing:                                                                                        |" << std::endl;
  std::cout << "| 0%     10%       20%       30%       40%       50%       60%       70%       80%       90%     100%|" << std::endl;
  std::cout << "|.........|.........|.........|.........|.........|.........|.........|.........|.........|..........|" << std::endl;
  std::cout << "|" << std::flush;
  int iFactors = 0;

  Setup* setup = Setup::instance();
  TrackFinding tf;

  SimpleEvent* event = 0;
  QMap<int, TH2D*> hists;

  for (unsigned int i = 0; i < nEntries; i++) {
    event = dc.event(i);
    
    // vector of all hits in this event
    QVector<Hit*> hits = QVector<Hit*>::fromStdVector(event->hits());
    QVector<Hit*> clusters;

    bool needToFindClusters = false;
    foreach(Hit* hit, hits) {
      if (dynamic_cast<Cluster*>(hit)) {
        clusters.push_back(hit);
        setup->layer(hit->position().z());
      }
      else {
        needToFindClusters = true;
      }
    }

    if (needToFindClusters) {
      setup->addHitsToLayers(hits);
      Layer* layer = setup->firstLayer();
      while(layer) {
        QVector<Cluster*> clustersHere = layer->clusters();
        foreach(Cluster* cluster, clustersHere)
          clusters.push_back(cluster);

        Cluster* cluster = layer->bestCluster();
        if (cluster)
          clusters.push_back(cluster);
        layer->clearHitsInDetectors();

        // update pointer
        layer = setup->nextLayer();
      }
    }

    // track finding
    //clusters = tf.findTrack(clusters);
    
    QMap<int, Hit*> map;
    foreach(Hit* cluster, clusters) {
      if (cluster->type() == Hit::tracker) {
        map[(int)cluster->position().z()] = cluster;
      }
    }

    int z0[4] = {-236, -69, 50, 217};
    int z1[4] = {-217, -50, 69, 236};
    for (int iLayer = 0; iLayer < 4; iLayer++) {
      if (map[z0[iLayer]] && map[z1[iLayer]]) {
        double x0 = 0.5*(map[z0[iLayer]]->position() + map[z0[iLayer]]->counterPosition()).x();
        double x1 = 0.5*(map[z1[iLayer]]->position() + map[z1[iLayer]]->counterPosition()).x();
        if (!hists[z0[iLayer]]) {
          char title[128];
          sprintf(title, "layer_%d", z0[iLayer]);
          hists[z0[iLayer]] = new TH2D(title, title, 100, -100, 100, 500, -50, 50);
          hists[z0[iLayer]]->GetXaxis()->SetTitle("x_{0} / mm");
          hists[z0[iLayer]]->GetYaxis()->SetTitle("x_{1} - x_{0} / mm");
        }
        hists[z0[iLayer]]->Fill(x0, x1-x0);
      }
    }

    if ( i > iFactors*nEntries/100. ) {
      std::cout << "#" << std::flush;
      iFactors++;
    }
  }

  std::cout << "|" << std::endl;
  std::cout << "+----------------------------------------------------------------------------------------------------+" << std::endl;

  foreach(TH2D* hist, hists) {
    TCanvas* can = new TCanvas;
    can->Draw();
    can->cd();
    hist->Draw("colz");
  }

  app.Run();

  return 0;
}
