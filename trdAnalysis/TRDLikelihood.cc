#include "TRDLikelihood.hh"

#include <math.h>

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QMutexLocker>
#include <QSettings>

#include <QDebug>

#include "TF1.h"
#include "TMath.h"

#include "Track.hh"
#include "TrackInformation.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "TRDCalculations.hh"

#include "AMS_PDFs/pfun.C"


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
    path += "/conf/";
  }
  else {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  m_pathToLikelihoodHistos = path + "trdLikelihoods/";

  m_trdLikelihoodSettings = new QSettings(path + "TRDLikelihood.conf", QSettings::IniFormat);

  double momBinWidth = 0.5;
  for (double lower = 0; lower < 10; lower += momBinWidth)
    m_defaultMomBins << QPair<double, double>(lower, lower+momBinWidth);

  initializeModuleLikelihoods();
}

TRDLikelihood::~TRDLikelihood(){
  //TODO: needed?

  qDeleteAll(m_positronLikelihood);
  qDeleteAll(m_protonLikelihood);

  qDeleteAll(m_protonModuleSumLikelihood);
  qDeleteAll(m_positronModuleSumLikelihood);

  QMap < QPair<double,double> , QMap<unsigned int,TH1D* > >::const_iterator iHisto = m_protonModuleLikelihood.constBegin();
  while(iHisto != m_protonModuleLikelihood.constEnd())
  {
    qDeleteAll(iHisto.value());
    ++iHisto;
  }

  iHisto = m_positronModuleLikelihood.constBegin();
  while(iHisto != m_positronModuleLikelihood.constEnd())
  {
    qDeleteAll(iHisto.value());
    ++iHisto;
  }


  QMap < QPair<double,double> , QMap<unsigned int,TF1* > >::const_iterator iFunctions = m_protonLHFuns.constBegin();
  while(iFunctions != m_protonLHFuns.constEnd())
  {
    qDeleteAll(iFunctions.value());
    ++iFunctions;
  }

  iFunctions = m_positronLHPFuns.constBegin();
  while(iFunctions != m_positronLHPFuns.constEnd())
  {
    qDeleteAll(iFunctions.value());
    ++iFunctions;
  }


  QMap < QPair<double,double> , QMap<unsigned int,QList<double>* > >::const_iterator iPars = m_protonLHPars.constBegin();
  while(iPars != m_protonLHPars.constEnd())
  {
    qDeleteAll(iPars.value());
    ++iPars;
  }

  iPars = m_positronLHPars.constBegin();
  while(iPars != m_positronLHPars.constEnd())
  {
    qDeleteAll(iPars.value());
    ++iPars;
  }

  TRDLikelihood::m_instance = 0;
}

TRDLikelihood* TRDLikelihood::instance()
{
  if (!m_instance) {
    QMutexLocker locker(&m_mutex);
    m_instance = new TRDLikelihood;
  }
  return m_instance;
}



void TRDLikelihood::initializeModuleLikelihoods(){

  for(int i = 0; i < m_defaultMomBins.size(); ++i)
  {
    QString momBinString = QString::number(m_defaultMomBins.at(i).first) + '_' + QString::number(m_defaultMomBins.at(i).second);

    Setup* setup = Setup::instance();
    const ElementIterator endIt = setup->lastElement();
    for (ElementIterator it = setup->firstElement(); it != endIt; ++it) {
      DetectorElement* element = *it;
      if (element->type() == DetectorElement::trd){
        unsigned int detID = element->id();

        //add parameters
        m_protonLHPars[m_defaultMomBins.at(i)].insert(detID, new QList <double>);
        m_positronLHPars[m_defaultMomBins.at(i)].insert(detID, new QList <double>);
        //add functions
        m_protonLHFuns[m_defaultMomBins.at(i)].insert(detID, new TF1("f_ppar",pfunperdaix,0.,100,6));
        m_positronLHPFuns[m_defaultMomBins.at(i)].insert(detID, new TF1("f_epar","(landau(0)+landau(3))"));


        QString titlePositronHisto = "positronLikeliHood "
                                     + momBinString
                                     + ' ' + QString::number(detID, 16) ;
        TH1D* positronLikelihoodHisto = new TH1D(qPrintable(titlePositronHisto), qPrintable(titlePositronHisto + ";TRD Signal;probability"), 400, 0, 100);
        m_positronModuleLikelihood[m_defaultMomBins.at(i)].insert(detID, positronLikelihoodHisto);

        QString titleProtonHisto = "protonLikeliHood "
                                     + momBinString
                                     + ' ' + QString::number(detID, 16) ;
        TH1D* protonLikelihoodHisto = new TH1D(qPrintable(titleProtonHisto), qPrintable(titleProtonHisto + ";TRD Signal;probability"), 400, 0, 25);
        m_protonModuleLikelihood[m_defaultMomBins.at(i)].insert(detID, protonLikelihoodHisto);



      }
    }

    QString titlePositronHisto = "positronLikeliHood SumOfModules " + momBinString;
    m_positronModuleSumLikelihood.insert(m_defaultMomBins.at(i), new TH1D(qPrintable(titlePositronHisto), qPrintable(titlePositronHisto+";TRD Signal;probability"), 1000, 0, 100));
    QString titleProtonHisto = "protonLikeliHood SumOfModules " + momBinString;
    m_protonModuleSumLikelihood.insert(m_defaultMomBins.at(i), new TH1D(qPrintable(titleProtonHisto), qPrintable(titleProtonHisto+";TRD Signal;probability"), 1000, 0, 100));

    titlePositronHisto = "positronLikeliHoods " + momBinString;
    m_positronLikelihood.insert(m_defaultMomBins.at(i), new TH1D(qPrintable(titlePositronHisto), qPrintable(titlePositronHisto+";-ln(L);probability"), 1000, 0, 100));
    titleProtonHisto = "protonLikeliHoods " + momBinString;
    m_protonLikelihood.insert(m_defaultMomBins.at(i), new TH1D(qPrintable(titleProtonHisto), qPrintable(titleProtonHisto+";-ln(L);probability"), 1000, 0, 100));

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

  //only use following momenta
  double pAbs = qAbs(p);
  if(pAbs < 0.5 || pAbs > 10.5){
    m_pNotInRange++;
    return false;
  }

  //find momentum bin
  QPair<double, double> momBin = findMomBin(pAbs, m_defaultMomBins);
  if( momBin == QPair<double, double>(0,0) ){
    m_pNotInRange++;
    return false;
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
  if (trdClusterHitsOnTrack.size() < 6)
    return false;

  /*TODO: -what if normalization to track length doesnt work?
  *       -what if not all layers have been hit (is there a different likelihood for a hit for different particles?)
  *       -....
  */

  //event passed and can be analyzed:

  //get energyDeposition in a Module and save the likelihoods which are parameterized as functions
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
        TF1* protonLHFun = m_protonLHFuns.value(momBin).value(moduleID);
        double protonLH = protonLHFun->Eval(signal);
        protonLikelihoods << protonLH;

        TF1* positronLHFun = m_positronLHPFuns.value(momBin).value(moduleID);
        double positronLH = positronLHFun->Eval(signal);
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
    m_positronLikelihood.value(momBin)->Fill(testValue);
  else
    m_protonLikelihood.value(momBin)->Fill(testValue);

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


void TRDLikelihood::addLearnEvent(const QVector<Hit*>&, const Track* track, const SimpleEvent* event){

  //check event:
  if ( event->MCInformation() == 0 )
    return;

  //get particle info:
  int pdgId = event->MCInformation()->pdgId();

  if ( !(qAbs(pdgId) == 2212 || qAbs(pdgId) == 11) )
    return;

  bool isProton = (pdgId == 2212);

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
  if(pAbs < 0.5 || pAbs > 10.5){
    m_pNotInRange++;
    return;
  }

  //find momentum bin
  QPair<double, double> momBin = findMomBin(pAbs, m_defaultMomBins);
  if( momBin == QPair<double, double>(0,0) ){
    m_pNotInRange++;
    return;
  }

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Hit*> trdClusterHitsOnTrack;


  //TODO: check for off track hits ?!?
  foreach(Hit* clusterHit, track->hits()){
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
          m_protonModuleLikelihood.value(momBin).value(moduleID)->Fill(signal);
          m_protonModuleSumLikelihood.value(momBin)->Fill(signal);
        }else{
          m_positronModuleLikelihood.value(momBin).value(moduleID)->Fill(signal);
          m_positronModuleSumLikelihood.value(momBin)->Fill(signal);
        }
      }
    }
  }
}


void TRDLikelihood::normalizeLikelihoodHistos(){

  //normalize sum histos
  QMap < QPair<double,double> , TH1D* >::const_iterator itSum = m_positronLikelihood.constBegin();
  while ( itSum !=  m_positronLikelihood.constEnd() )
  {
    (itSum.value())->Sumw2();
    (itSum.value())->Scale(1.0 / (itSum.value())->Integral("width"));
    ++itSum;
  }

  itSum = m_protonLikelihood.constBegin();
  while ( itSum !=  m_protonLikelihood.constEnd() )
  {
    (itSum.value())->Sumw2();
    (itSum.value())->Scale(1.0 / (itSum.value())->Integral("width"));
    ++itSum;
  }


  //normalize module histos
  QMap < QPair<double,double> , QMap<unsigned int,TH1D* > >::const_iterator itMom = m_protonModuleLikelihood.constBegin();
  while( itMom != m_protonModuleLikelihood.constEnd())
  {
    QMap<unsigned int,TH1D* >::const_iterator itMod = (itMom.value()).constBegin();
    while( itMod != (itMom.value()).constEnd())
    {
      (itMod.value())->Sumw2();
      (itMod.value())->Scale(1.0 / (itSum.value())->Integral("width"));
    }
    ++itMom;
  }

  itMom = m_positronModuleLikelihood.constBegin();
  while( itMom != m_positronModuleLikelihood.constEnd())
  {
    QMap<unsigned int,TH1D* >::const_iterator itMod = (itMom.value()).constBegin();
    while( itMod != (itMom.value()).constEnd())
    {
      (itMod.value())->Sumw2();
      (itMod.value())->Scale(1.0 / (itSum.value())->Integral("width"));
    }
    ++itMom;
  }

}




void TRDLikelihood::fitLikelihoodHistos()
{
  //fit proton module histos
  QMap < QPair<double,double> , QMap<unsigned int,TH1D* > >::const_iterator itMom = m_protonModuleLikelihood.constBegin();
  while( itMom != m_protonModuleLikelihood.constEnd())
  {
    QMap<unsigned int,TH1D* >::const_iterator itMod = (itMom.value()).constBegin();
    while( itMod != (itMom.value()).constEnd())
    {
      TF1 *f_ppar = m_protonLHFuns.value(itMom.key()).value(itMod.key());
      f_ppar->SetParameters(4.32391,2.23676,1.02281,0.788797,6,-0.1);
      for (int i = 4; i < 5; i++)
        f_ppar->SetParLimits(i,f_ppar->GetParameter(i),f_ppar->GetParameter(i));
      (itMod.value())->Fit(f_ppar, "Q");

      double integral = f_ppar->Integral(0,100);
      f_ppar->SetParameter(0, f_ppar->GetParameter(0) / integral);
    }
    ++itMom;
  }

  //fit positron module histos
  itMom = m_protonModuleLikelihood.constBegin();
  while( itMom != m_protonModuleLikelihood.constEnd())
  {
    QMap<unsigned int,TH1D* >::const_iterator itMod = (itMom.value()).constBegin();
    while( itMod != (itMom.value()).constEnd())
    {
      TF1 *f_epar = m_positronLHPFuns.value(itMom.key()).value(itMod.key());
      f_epar->SetParameters(0.533941,3,0.90618,0.177358,7,4.80789);
      f_epar->SetParLimits(1,2,4);
      f_epar->SetParLimits(4,5,10);
      (itMod.value())->Fit(f_epar, "Q");

      double integral = f_epar->Integral(0,250);
      f_epar->SetParameter(0, f_epar->GetParameter(0) / integral);
    }
    ++itMom;
  }
}



void TRDLikelihood::saveLikelihoodHistos(){
 /*
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
  */
}



void TRDLikelihood::saveFunParameters()
{
  m_trdLikelihoodSettings->beginGroup("protons");
  QMap < QPair<double,double> , QMap<unsigned int,QList<double>* > >::const_iterator iMom
      = m_protonLHPars.constBegin();
  while (iMom != m_protonLHPars.constEnd())
  {
    //QPair<QVariant, QVariant> momBin(iMom.key().first, iMom.key().second);
    m_trdLikelihoodSettings->beginGroup(QString::number(iMom.key().first) + '_' + QString::number(iMom.key().second));
    QMap<unsigned int, QList<double>* >::const_iterator iMod = iMom.value().constBegin();
    while (iMod != iMom.value().constEnd())
    {
      QList<QVariant> pars;
      for(int j = 0; j < (iMod.value())->size(); ++j)
        pars << (iMod.value()->at(j));
      m_trdLikelihoodSettings->setValue(QString::number(iMod.key(),16), pars);

      ++iMod;
    }
    m_trdLikelihoodSettings->endGroup();
    ++iMom;
  }
  m_trdLikelihoodSettings->endGroup();

  m_trdLikelihoodSettings->beginGroup("positrons");
  iMom = m_positronLHPars.constBegin();
  while (iMom != m_positronLHPars.constEnd())
  {
    m_trdLikelihoodSettings->beginGroup(QString::number(iMom.key().first) + '_' + QString::number(iMom.key().second));
    QMap<unsigned int, QList<double>* >::const_iterator iMod = iMom.value().constBegin();
    while (iMod != iMom.value().constEnd())
    {
      QList<QVariant> pars;
      for(int j = 0; j < (iMod.value())->size(); ++j)
        pars << (iMod.value()->at(j));
      m_trdLikelihoodSettings->setValue(QString::number(iMod.key(),16), pars);

      ++iMod;
    }
    m_trdLikelihoodSettings->endGroup();
    ++iMom;
  }
  m_trdLikelihoodSettings->sync();

  qDebug() << "done writing settings file.";
}

void TRDLikelihood::loadFunParameters()
{
  m_trdLikelihoodSettings->beginGroup("protons");
  const QStringList momBinsProtonsStrings = m_trdLikelihoodSettings->childGroups();
  foreach (QString momBinString, momBinsProtonsStrings){
    m_trdLikelihoodSettings->beginGroup(momBinString);
    QPair<double, double> momBin;
    QStringList momBinValueStrings = momBinString.split('_');
    momBin.first = momBinValueStrings.at(0).toDouble() ;
    momBin.second = momBinValueStrings.at(1).toDouble() ;
    QMap<unsigned int, QList<double>* >& protonPars = m_protonLHPars[momBin];
    QStringList moduleIDStrings = m_trdLikelihoodSettings->childKeys();
    foreach(QString moduleIDString, moduleIDStrings)
    {
      bool ok;
      int moduleID = moduleIDString.toInt(&ok,16);
      QList<double>* modulePars = protonPars[moduleID];
      if (modulePars)
        modulePars->clear();
      else
      {
        modulePars = new QList<double>;
        protonPars.insert(moduleID, modulePars);
      }
      QList<QVariant> pars = m_trdLikelihoodSettings->value(moduleIDString).toList();
      for(int j = 0; j < pars.size(); ++j)
        modulePars->append(pars.at(j).toDouble());

      qDebug() << "protons read mom bin " << momBin <<  " module 0x" + QString::number(moduleID,16) + ": " << *modulePars;
    }
    m_trdLikelihoodSettings->endGroup();
  }
  m_trdLikelihoodSettings->endGroup();


  m_trdLikelihoodSettings->beginGroup("positrons");
  const QStringList momBinsPositronsStrings = m_trdLikelihoodSettings->childGroups();
  foreach (QString momBinString, momBinsPositronsStrings){
    m_trdLikelihoodSettings->beginGroup(momBinString);
    QPair<double, double> momBin;
    QStringList momBinValueStrings = momBinString.split('_');
    momBin.first = momBinValueStrings.at(0).toDouble() ;
    momBin.second = momBinValueStrings.at(1).toDouble() ;
    QMap<unsigned int, QList<double>* >& positronPars = m_positronLHPars[momBin];
    QStringList moduleIDStrings = m_trdLikelihoodSettings->childKeys();
    foreach(QString moduleIDString, moduleIDStrings)
    {
      bool ok;
      int moduleID = moduleIDString.toInt(&ok,16);
      QList<double>* modulePars = positronPars[moduleID];
      if (modulePars)
        modulePars->clear();
      else
      {
        modulePars = new QList<double>;
        positronPars.insert(moduleID, modulePars);
      }
      QList<QVariant> pars = m_trdLikelihoodSettings->value(moduleIDString).toList();
      for(int j = 0; j < pars.size(); ++j)
        modulePars->append(pars.at(j).toDouble());

      qDebug() << "positrons read mom bin" << momBin <<  "module 0x" + QString::number(moduleID,16) + ":" << *modulePars;
    }
    m_trdLikelihoodSettings->endGroup();
  }
  m_trdLikelihoodSettings->endGroup();

}


QPair<double,double> TRDLikelihood::findMomBin (const double mom, const QList< QPair<double,double> > & momBins)
{
  QList< QPair<double,double> >::const_iterator it = momBins.constBegin();
  while(it != momBins.constEnd())
  {
    if ( (*it).first <= mom && mom < (*it).second )
      return *it;
    ++it;
  }

  return QPair<double,double> (0,0);
}
