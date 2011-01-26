#include "TRDLikelihood.hh"

#include <math.h>

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QMutexLocker>

#include "Track.hh"
#include "TrackInformation.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "TRDCalculations.hh"


TRDLikelihood* TRDLikelihood::m_instance = 0;
QMutex TRDLikelihood::m_mutex;


TRDLikelihood::TRDLikelihood()
{

  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_PATH=*"));
  QString path = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    path = entry.split("=").at(1);
    path += "/conf/trdLikelihoods/";
  }
  else {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  m_pathToLikelihoodHistos = path ;

  initializeModuleLikelihoods();

  m_positronLikelihood = new TH1D("positronLikelihood", "positronLikelihood;probability;L", 100, 0, 1);
  m_protonLikelihood = new TH1D("protonLikelihood", "protonLikelihood;probability;L", 100, 0, 1);
}

TRDLikelihood::~TRDLikelihood(){
  //TODO: needed?
  qDeleteAll(m_positronModuleLikelihood);
  qDeleteAll(m_protonModuleLikelihood);

  delete m_positronLikelihood;
  delete m_protonLikelihood;

  TRDLikelihood::m_instance = 0;
}

TRDLikelihood* TRDLikelihood::instance()
{
  QMutexLocker locker(&m_mutex);
  if (!m_instance) m_instance = new TRDLikelihood;
  return m_instance;
}



void TRDLikelihood::initializeModuleLikelihoods(){
  Setup* setup = Setup::instance();
  DetectorElement* element = setup->firstElement();
  while(element) {
    if (element->type() == DetectorElement::trd){
      unsigned int detID = element->id();
      QString titlePositronHisto = "Positron Likelihood for module 0x" + QString::number(detID, 16) ;
      TH1D* positronLikelihoodHisto = new TH1D(qPrintable(titlePositronHisto), qPrintable(titlePositronHisto + ";TRD Signal;Likelihood"), 100, 0, 50);
      m_positronModuleLikelihood.insert(detID, positronLikelihoodHisto);

      QString titleProtonHisto = "Proton Likelihood for module 0x" + QString::number(detID, 16) ;
      TH1D* protonLikelihoodHisto = new TH1D(qPrintable(titleProtonHisto), qPrintable(titleProtonHisto + ";TRD Signal;Likelihood"), 100, 0, 50);
      m_protonModuleLikelihood.insert(detID, protonLikelihoodHisto);
    }
    element = setup->nextElement();
  }

}

bool TRDLikelihood::analyzeEvent(const QVector<Hit*>& hits, const Track* track, const SimpleEvent*, bool& isPositronish){
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return false;

  //check if all tracker layers have a hit
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return false;

  //check if track was inside of magnet
  if (!(flags & TrackInformation::InsideMagnet))
    return false;

  //get the reconstructed momentum
  double p = track->p(); //GeV

  //only use following momenta 0.5 < |p| < 5
  double pAbs = qAbs(p);
  if(pAbs < 0.5 || pAbs > 5)
    return false;

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
    return false;

  /*TODO: -what if normalization to track length doesnt work?
  *       -what if not all layers have been hit (is there a different likelihood for a hit for different particles?)
  *       -....
  */

  //event passed and can be analyzed:

  //get energyDeposition in a Module and save the likelihoods which are stored in histos
  QVector <double> positronLikelihoods;
  QVector <double> protonLikelihoods;

  foreach(Hit* clusterHit, trdClusterHitsOnTrack){
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    foreach(Hit* hit, cluster->hits()){
      double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
      if(distanceInTube > 0){
        unsigned int tubeID = hit->detId();
        unsigned int moduleID = 0xFFF0 & tubeID;
        double signal = (hit->signalHeight() / distanceInTube) ;

        //get likelihoods:

        TH1D* protonHisto = m_protonModuleLikelihood.value(moduleID);
        protonLikelihoods <<  protonHisto->GetBinContent( protonHisto->GetBin(signal) );

        TH1D* positronHisto = m_positronModuleLikelihood.value(moduleID);
        positronLikelihoods << positronHisto->GetBinContent( positronHisto->GetBin(signal) );

      }
    }
  }

  //calculate combined likelihoods
  double protonLikelihood = 0;
  foreach(double moduleLikelihood, protonLikelihoods)
    protonLikelihood *= moduleLikelihood;
  protonLikelihood = pow(protonLikelihood, 1.0 / protonLikelihoods.size() );

  double positronLikelihood = 0;
  foreach(double moduleLikelihood, positronLikelihoods)
    positronLikelihood *= moduleLikelihood;
  positronLikelihood = pow(positronLikelihood, 1.0 / positronLikelihoods.size() );

  //fill into histos
  m_protonLikelihood->Fill(protonLikelihood);
  m_positronLikelihood->Fill(positronLikelihood);

  //test:
  double testValue = positronLikelihood / ( positronLikelihood + protonLikelihood) ;

  if ( testValue > 0.5){
    isPositronish = true;
  }else{
    isPositronish = false;
  }

  return true;

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
  if(pAbs < 0.5 || pAbs > 5)
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
        unsigned int tubeID = hit->detId();
        unsigned int moduleID = 0xFFF0 & tubeID;
        double signal = (hit->signalHeight() / distanceInTube) ;

        //now fill into correct histo
        if (isProton) {
          m_protonModuleLikelihood.value(moduleID)->Fill(signal);
        }else{
          m_positronModuleLikelihood.value(moduleID)->Fill(signal);
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


void TRDLikelihood::saveLikelihoodHistos(){

  //save proton likelihoods
  foreach (TH1D* histo, m_protonModuleLikelihood){
    histo->SaveAs(qPrintable(m_pathToLikelihoodHistos + QString(histo->GetTitle()).replace(" ", "_") + ".root"));
  }

  //save positron likelihoods
  foreach (TH1D* histo, m_positronModuleLikelihood){
    histo->SaveAs(qPrintable(m_pathToLikelihoodHistos + QString(histo->GetTitle()).replace(" ", "_") + ".root"));
  }

}
