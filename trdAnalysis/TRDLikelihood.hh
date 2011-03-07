#ifndef TRDLIKELIHOOD_H
#define TRDLIKELIHOOD_H

#include <QMap>
#include <QPair>
#include <QVector>
#include <QString>
#include <QMutex>

#include <TH1D.h>

class QSettings;


class Track;
class SimpleEvent;
class Hit;

class TRDLikelihood
{
public:
    ~TRDLikelihood();
    static TRDLikelihood* instance();

    bool analyzeEvent(const QVector<Hit*>& hits, const Track* track, const SimpleEvent*, bool& isPositronish);

    void addLearnEvent(const QVector<Hit*>& hits, const Track* track, const SimpleEvent*);

    TH1D* protonModuleLikelihoodHisto(QPair<double,double> momBin, unsigned int moduleID)    {return m_protonModuleLikelihood.value(momBin).value(moduleID) ;}
    TH1D* positronModuleLikelihoodHisto(QPair<double,double> momBin, unsigned int moduleID)  {return m_positronModuleLikelihood.value(momBin).value(moduleID) ;}

    void saveLikelihoodHistos();
    void loadLikelihoodHistos();

    void saveFunParameters();
    void loadFunParameters();

    bool saved() {return m_saved;}

private:
  TRDLikelihood();

  static TRDLikelihood* m_instance;

  bool m_saved;

  double m_likelihoodCut;

  int m_noTrackfit;
  int m_notGoodChi2;
  int m_notInMagnet;
  int m_albedo;
  int m_pNotInRange;
  int m_notEnoughTRDCluster;
  int m_eventsOK;


  int m_protonCorrect;
  int m_protonFalse;
  int m_positronCorrect;
  int m_positronFalse;

  void initializeModuleLikelihoods() ;
  void normalizeLikelihoodHistos() ;
  void fitLikelihoodHistos() ;

  QString m_pathToLikelihoodHistos;

  QList< QPair<double,double> > m_defaultMomBins;

  QPair<double,double> findMomBin (const double mom, const QList< QPair<double,double> > & momBins);

  QMap < QPair<double,double> , TH1D* > m_positronLikelihood;
  QMap < QPair<double,double> , TH1D* > m_protonLikelihood;

  QMap < QPair<double,double> , TH1D* > m_protonModuleSumLikelihood;
  QMap < QPair<double,double> , TH1D* > m_positronModuleSumLikelihood;

  QMap < QPair<double,double> , QMap<unsigned int,TH1D* > > m_protonModuleLikelihood;
  QMap < QPair<double,double> , QMap<unsigned int,TH1D* > > m_positronModuleLikelihood;

  QMap < QPair<double,double> , QMap<unsigned int,TF1* > > m_protonLHFuns;
  QMap < QPair<double,double> , QMap<unsigned int,TF1* > > m_positronLHPFuns;

  QMap < QPair<double,double> , QMap<unsigned int,QList<double>* > > m_protonLHPars;
  QMap < QPair<double,double> , QMap<unsigned int,QList<double>* > > m_positronLHPars;

  QSettings* m_trdLikelihoodSettings;

  static QMutex m_mutex;
};

#endif // TRDLIKELIHOOD_H
