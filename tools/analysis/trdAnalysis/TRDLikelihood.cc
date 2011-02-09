#include "TRDLikelihood.hh"

#include <math.h>

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QMutexLocker>

#include <QDebug>

#include "TF1.h"

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

TRDLikelihood::TRDLikelihood():
    m_saved(false),
    m_likelihoodCut(0.5),
    m_noTrackfit(0),
    m_notGoodChi2(0),
    m_notInMagnet(0),
    m_albedo(0),
    m_pNotInRange(0),
    m_notEnoughTRDCluster(0),
    m_eventsOK(0),
    m_protonCorrect(0),
    m_protonFalse(0),
    m_positronCorrect(0),
    m_positronFalse(0)
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

  m_positronLikelihood = new TH1D("positronLikelihood", "positronLikelihood;-ln(L);probability", 100, 0, 3);
  m_protonLikelihood = new TH1D("protonLikelihood", "protonLikelihood;-ln(L);probability", 100, 0, 3);
}

TRDLikelihood::~TRDLikelihood(){
  //TODO: needed?
  qDeleteAll(m_positronModuleLikelihood);
  qDeleteAll(m_protonModuleLikelihood);

  delete m_positronLikelihood;
  delete m_protonLikelihood;

  delete m_positronModuleSumLikelihood;
  delete m_protonModuleSumLikelihood;

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
      TH1D* positronLikelihoodHisto = new TH1D(qPrintable(titlePositronHisto), qPrintable(titlePositronHisto + ";TRD Signal;Likelihood"), 100, 0, 25);
      m_positronModuleLikelihood.insert(detID, positronLikelihoodHisto);

      QString titleProtonHisto = "Proton Likelihood for module 0x" + QString::number(detID, 16) ;
      TH1D* protonLikelihoodHisto = new TH1D(qPrintable(titleProtonHisto), qPrintable(titleProtonHisto + ";TRD Signal;Likelihood"), 100, 0, 25);
      m_protonModuleLikelihood.insert(detID, protonLikelihoodHisto);
    }
    element = setup->nextElement();
  }

  m_positronModuleSumLikelihood = new TH1D("Positron Likelihood for Sum of Modules", "Positron Likelihood for Sum of Modules;TRD Signal;Likelihood", 250, 0, 25);
  m_protonModuleSumLikelihood = new TH1D("Proton Likelihood for Sum of Modules", "Proton Likelihood for Sum of Modules;TRD Signal;Likelihood", 250, 0, 25);

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

        //TH1D* protonHisto = m_protonModuleLikelihood.value(moduleID);
        TH1D* protonHisto = m_protonModuleSumLikelihood;
        double protonLH = protonHisto->GetBinContent( protonHisto->FindBin(signal) );
        if(protonLH > 0)
          protonLikelihoods <<  protonLH;

        //TH1D* positronHisto = m_positronModuleLikelihood.value(moduleID);
        TH1D* positronHisto = m_positronModuleSumLikelihood;
        double positronLH = positronHisto->GetBinContent( positronHisto->FindBin(signal) );
        if (positronLH > 0)
          positronLikelihoods << positronLH;

      } else {
        //m_noDistanceInTube++;
      }
    }
  }

  //calculate combined likelihoods
  double protonLikelihood = 1;
  foreach(double moduleLikelihood, protonLikelihoods)
    protonLikelihood *= moduleLikelihood;
  protonLikelihood = pow(protonLikelihood, 1.0 / protonLikelihoods.size() );

  double positronLikelihood = 1;
  foreach(double moduleLikelihood, positronLikelihoods)
    positronLikelihood *= moduleLikelihood;
  positronLikelihood = pow(positronLikelihood, 1.0 / positronLikelihoods.size() );



  //fill into histos
  //m_protonLikelihood->Fill(protonLikelihood);
  //m_positronLikelihood->Fill(positronLikelihood);

  //test:
  double testValue = -log(positronLikelihood / ( positronLikelihood + protonLikelihood)) ;

  bool isPositron = isPositronish;

  if (isPositron)
    m_positronLikelihood->Fill(testValue);
  else
    m_protonLikelihood->Fill(testValue);

  if ( testValue > 0.5){
    isPositronish = false;
  }else{
    isPositronish = true;
  }

  m_protonCorrect += (!isPositron && !isPositronish);
  m_protonFalse += (!isPositron && isPositronish);
  m_positronCorrect += (isPositron && isPositronish);
  m_positronFalse += (isPositron && !isPositronish);


  return true;

}


void TRDLikelihood::addLearnEvent(const QVector<Hit*>& hits, const Track* track, const SimpleEvent*, bool isProton){
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood()){
    m_noTrackfit++;
    return;
  }

  TrackInformation::Flags flags = track->information()->flags();
  //check if track was inside of magnet
  if (!(flags & TrackInformation::InsideMagnet)){
    m_notInMagnet++;
    return;
  }

  //check if good fit
  if ( track->chi2() / track->ndf() > 4.){
    m_notGoodChi2++;
    return;
  }

  //check albedo
  if ((flags & TrackInformation::Albedo)){
    m_albedo++;
    return;
  }


  //get the reconstructed momentum
  double p = track->p(); //GeV

  //only use following momenta 0.5 < |p| < 5
  double pAbs = qAbs(p);
  if(pAbs < 0.5 || pAbs > 5){
    m_pNotInRange++;
    return;
  }

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Hit*> trdClusterHitsOnTrack;


  //TODO: check for off track hits ?!?
  foreach(Hit* clusterHit, hits){
    if (clusterHit->type() == Hit::trd)
      trdClusterHitsOnTrack.push_back(clusterHit);
  }


  //filter: only use events with 6 trd hits
  if (trdClusterHitsOnTrack.size() < 6){
    m_notEnoughTRDCluster++;
    return;
  }

  m_eventsOK++;


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
          m_protonModuleSumLikelihood->Fill(signal);
        }else{
          m_positronModuleLikelihood.value(moduleID)->Fill(signal);
          m_positronModuleSumLikelihood->Fill(signal);
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
    TF1 *f_ppar= new TF1("f_epar","(landau(0))");
    f_ppar->SetParameters(0.533941,1.04519,0.90618);
    histo->Fit(f_ppar);
  }

  m_protonModuleSumLikelihood->Sumw2();
  m_protonModuleSumLikelihood->Scale(1.0 / m_protonModuleSumLikelihood->Integral());
  TF1 *f_ppar= new TF1("f_epar","(landau(0))");
  f_ppar->SetParameters(0.533941,1.04519,0.90618);
  m_protonModuleSumLikelihood->Fit(f_ppar);


  //normalize positron likelihoods
  foreach (TH1D* histo, m_positronModuleLikelihood){
    histo->Sumw2();
    histo->Scale(1.0 / histo->Integral());
    TF1 *f_epar= new TF1("f_epar","(landau(0)+landau(3))");
    f_epar->SetParameters(0.533941,1.04519,0.90618,0.177358,3.7248,4.80789);
    f_epar->SetParLimits(1,0.8,1.2);
    f_epar->SetParLimits(4,2,4);
    histo->Fit(f_epar);
  }

  m_positronModuleSumLikelihood->Sumw2();
  m_positronModuleSumLikelihood->Scale(1.0 / m_positronModuleSumLikelihood->Integral());
  TF1 *f_epar= new TF1("f_epar","(landau(0)+landau(3))");
  f_epar->SetParameters(0.533941,1.04519,0.90618,0.177358,3.7248,4.80789);
  f_epar->SetParLimits(1,0.8,1.2);
  f_epar->SetParLimits(4,2,4);
  m_positronModuleSumLikelihood->Fit(f_epar);

}


void TRDLikelihood::saveLikelihoodHistos(){

  if (m_saved){
    m_protonLikelihood->Sumw2();
    m_protonLikelihood->Scale(1.0 / m_protonLikelihood->Integral());
    m_protonLikelihood->SaveAs("protonLikelihoodHisto.root");
    m_positronLikelihood->Sumw2();
    m_positronLikelihood->Scale(1.0 / m_positronLikelihood->Integral());
    m_positronLikelihood->SaveAs("positronLikelihoodHisto.root");

    qDebug() << m_protonCorrect <<  m_protonFalse << m_positronCorrect << m_positronFalse;
    qDebug() << "for likelihood cut = " << m_likelihoodCut;
    qDebug() << "positron efficiency = " << (double)(m_positronCorrect) / (m_positronCorrect + m_positronFalse);
    qDebug() << "proton rejection = " << (double)(m_protonCorrect + m_protonFalse) / m_protonFalse;

    return;
  }

  normalizeLikelihoodHistos();

  //save proton likelihoods
  foreach (TH1D* histo, m_protonModuleLikelihood){
    histo->SaveAs(qPrintable(m_pathToLikelihoodHistos + QString(histo->GetTitle()).replace(" ", "_") + ".root"));
  }
  m_protonModuleSumLikelihood->SaveAs(qPrintable(m_pathToLikelihoodHistos + QString(m_protonModuleSumLikelihood->GetTitle()).replace(" ", "_") + ".root"));

  //save positron likelihoods
  foreach (TH1D* histo, m_positronModuleLikelihood){
    histo->SaveAs(qPrintable(m_pathToLikelihoodHistos + QString(histo->GetTitle()).replace(" ", "_") + ".root"));
  }
  m_positronModuleSumLikelihood->SaveAs(qPrintable(m_pathToLikelihoodHistos + QString(m_positronModuleSumLikelihood->GetTitle()).replace(" ", "_") + ".root"));

  int totalEvents = m_noTrackfit + m_notInMagnet + m_notGoodChi2 + m_albedo + m_pNotInRange + m_notEnoughTRDCluster + m_eventsOK;

  qDebug() << "total: " << totalEvents;
  qDebug() << "after trackfit: " << (totalEvents-=m_noTrackfit);
  qDebug() << "after magnet cut: " << (totalEvents-=m_notInMagnet);
  qDebug() << "after chi2 cut: " << (totalEvents-=m_notGoodChi2);
  qDebug() << "after albedo cut: " << (totalEvents-=m_albedo);
  qDebug() << "after p cut: " << (totalEvents-=m_pNotInRange);
  qDebug() << "after trd cut: " << (totalEvents-=m_notEnoughTRDCluster);

  m_saved = true;

}
