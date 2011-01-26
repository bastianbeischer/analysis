#include "TRDLikelihood.hh"

#include <QString>

#include "Track.hh"
#include "TrackInformation.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include "TRDCalculations.hh"

TRDLikelihood::TRDLikelihood()
{
  initializeModuleLikelihoods();
}



void TRDLikelihood::initializeModuleLikelihoods(){
  Setup* setup = Setup::instance();
  DetectorElement* element = setup->firstElement();
  while(element) {
    if (element->type() == DetectorElement::trd){
      unsigned int detID = element->id();

      QString titlePositronHisto = "Positron Likelihood for module 0x" + QString::number(detID, 16) ;
      TH1D* positronLikelihoodHisto = new TH1D(qPrintable(titlePositronHisto), qPrintable(titlePositronHisto + ";TRD Signal;Likelihood"), 100, 0, 25);
      m_positronModuleLikelihood.insert(detID, positronLikelihoodHisto);

      QString titleProtonHisto = "Proton Likelihood for module 0x" + QString::number(detID, 16) ;
      TH1D* protonLikelihoodHisto = new TH1D(qPrintable(titleProtonHisto), qPrintable(titleProtonHisto + ";TRD Signal;Likelihood"), 100, 0, 25);
      m_protonModuleLikelihood.insert(detID, protonLikelihoodHisto);

      element = setup->nextElement();
    }
  }

}


void TRDLikelihood::addLearnEvent(const QVector<Hit*>& hits, const Track* track, const SimpleEvent*, bool isProton){
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //check if all tracker layers have a hit
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  //check if track was inside of magnet
  if (!(flags & TrackInformation::InsideMagnet))
    return;

  //get the reconstructed momentum
  double p = track->p(); //GeV

  //only use following momenta 0.5 < |p| < 5
  double pAbs = qAbs(p);
  if(qAbs(p) < 0.5 || qAbs(p) > 5)
    return;

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Hit*> trdClusterHitsOnTrack;


  //TODO: check for off track hits ?!?
  foreach(Hit* clusterHit, hits){
    if (clusterHit->type() == Hit::trd)
      trdClusterHitsOnTrack.push_back(clusterHit);
  }


  //filter: only use events with 6 trd hits
  if (trdClusterHitsOnTrack.size() < 6)
    return;


  foreach(Hit* clusterHit, trdClusterHitsOnTrack){
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    foreach(Hit* hit, cluster->hits()){
      double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
      if(distanceInTube > 0){
        unsigned int detID = hit->detId();
        double signal = (hit->signalHeight() / distanceInTube) ;

        //now fill into correct histo
        if (isProton) {
          m_protonModuleLikelihood.value(detID)->Fill(signal);
        }else{
          m_positronModuleLikelihood.value(detID)->Fill(signal);
        }
      }
    }
  }
}


void TRDLikelihood::normalizeLikelihoodHistos(){

  //TODO: prevent several normalizations or handle them correctly

  //normalize proton likelihoods
  foreach (TH1D* histo, m_protonModuleLikelihood){
    histo->Sumw2();
    histo->Scale(1.0 / histo->Integral());
  }

  //normalize positron likelihoods
  foreach (TH1D* histo, m_positronModuleLikelihood){
    histo->Sumw2();
    histo->Scale(1.0 / histo->Integral());
  }

}
