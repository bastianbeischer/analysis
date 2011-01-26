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

    void addLearnEvent(const QVector<Hit*>& hits, const Track* track, const SimpleEvent*, bool isProton);

    TH1D* protonModuleLikelihoodHisto(unsigned int moduleID)    {return m_protonModuleLikelihood.value(moduleID) ;}
    TH1D* positronModuleLikelihoodHisto(unsigned int moduleID)  {return m_positronModuleLikelihood.value(moduleID) ;}

    void saveLikelihoodHistos();
    void loadLikelihoodHistos();

private:
  TRDLikelihood();

  static TRDLikelihood* m_instance;

  QMap <unsigned int, TH1D*> m_positronModuleLikelihood ;
  QMap <unsigned int, TH1D*> m_protonModuleLikelihood ;

  void initializeModuleLikelihoods() ;
  void normalizeLikelihoodHistos() ;

  QString m_pathToLikelihoodHistos;

  static QMutex m_mutex;
};

#endif // TRDLIKELIHOOD_H
