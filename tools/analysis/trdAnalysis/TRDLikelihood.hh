#ifndef TRDLIKELIHOOD_H
#define TRDLIKELIHOOD_H

#include <QMap>
#include <QVector>
#include <QString>
#include <QMutex>

#include <TH1D.h>


class Track;
class SimpleEvent;
class Hit;

class TRDLikelihood
{
public:
    ~TRDLikelihood();
    static TRDLikelihood* instance();

    bool analyzeEvent(const QVector<Hit*>& hits, const Track* track, const SimpleEvent*, bool& isPositronish);

    void addLearnEvent(const QVector<Hit*>& hits, const Track* track, const SimpleEvent*, bool isProton);

    TH1D* protonModuleLikelihoodHisto(unsigned int moduleID)    {return m_protonModuleLikelihood.value(moduleID) ;}
    TH1D* positronModuleLikelihoodHisto(unsigned int moduleID)  {return m_positronModuleLikelihood.value(moduleID) ;}

    void saveLikelihoodHistos();
    void loadLikelihoodHistos();

    bool saved() {return m_saved;}

private:
  TRDLikelihood();

  static TRDLikelihood* m_instance;

  bool m_saved;

  double m_likelihoodCut;

  int m_noTrackfit;
  int m_notAllTrackerLayers;
  int m_notInMagnet;
  int m_pNotInRange;
  int m_notEnoughTRDCluster;


  int m_protonCorrect;
  int m_protonFalse;
  int m_positronCorrect;
  int m_positronFalse;


  QMap <unsigned int, TH1D*> m_positronModuleLikelihood ;
  QMap <unsigned int, TH1D*> m_protonModuleLikelihood ;

  TH1D* m_positronModuleSumLikelihood;
  TH1D* m_protonModuleSumLikelihood;


  TH1D* m_positronLikelihood;
  TH1D* m_protonLikelihood;

  void initializeModuleLikelihoods() ;
  void normalizeLikelihoodHistos() ;

  QString m_pathToLikelihoodHistos;

  static QMutex m_mutex;
};

#endif // TRDLIKELIHOOD_H
