#include "TRDLikelihoods.hh"
#include "TRDReconstruction.hh"
#include "Helpers.hh"

#include <QMutex>
#include <QMutexLocker>
#include <QSettings>
#include <QStringList>

#include <QDebug>

#include <TF1.h>
#include <TMath.h>

double functionNonTR(double *x, double *par)
{
  /**
     p[0] normalization factor
     p[1] mopv
     p[2] sigma of rising landau flank
     p[3] sigma of falling landau flank
     p[4] linear term in exponential
     p[5] quadratic term in exponential
     p[6] cubic term in exponential
     p[7] const for gauss
     p[8] mean of gauss
     p[9] sigma of gauss
  **/
  if (x[0]<par[1])
    return par[0]*TMath::Landau(x[0],par[1],par[2])*TMath::Exp(par[4]*x[0]+par[5]*x[0]*x[0]+par[6]*x[0]*x[0]*x[0])+ par[7]*TMath::Gaus(x[0],0.,par[8]);
  else
    return par[0]*TMath::Landau(x[0],par[1],par[3])*TMath::Exp(par[4]*x[0]+par[5]*x[0]*x[0]+par[6]*x[0]*x[0]*x[0])+ par[7]*TMath::Gaus(x[0],0.,par[8]);
}


TRDLikelihoods* TRDLikelihoods::s_instance = 0;
QMutex TRDLikelihoods::s_mutex;

TRDLikelihoods* TRDLikelihoods::instance()
{
  if (!s_instance) {
    QMutexLocker locker(&s_mutex);
    if (!s_instance)
      s_instance = new TRDLikelihoods();
  }
  return s_instance;
}

TRDLikelihoods::TRDLikelihoods()
  : m_normalizedToLength(TRDReconstruction::s_calculateLengthInTube)
  , m_LHFunctionTR(prototypeLHFunctionTR())
  , m_LHFunctionNonTR(prototypeLHFunctionNonTR())
{
  for (int i = 0; i < 8; ++i)
    m_LHFunctionTRLayer << prototypeLHFunctionTR();
  
  QString path = Helpers::analysisPath() + "/conf/";
  m_trdLikelihoodSettings = new QSettings(path + "TRDLikelihood.conf", QSettings::IniFormat);

  loadLHs();
}

TF1* TRDLikelihoods::prototypeLHFunctionNonTR()
{
  /**
     p[0] normalization factor
     p[1] mopv
     p[2] sigma of rising landau flank
     p[3] sigma of falling landau flank
     p[4] linear term in exponential
     p[5] quadratic term in exponential
     p[6] cubic term in exponential
     p[7] const for gauss
     p[8] mean of gauss
     p[9] sigma of gauss
  **/

  TF1* LHFun = new TF1("TRDLikelihoodNonTR", functionNonTR, 0., 100., 9);
  //set default values:
  LHFun->SetNpx(1000);
  if (m_normalizedToLength)
    LHFun->SetParameters(7.5, 0.34, 0.115, 0.135, -0.0594475, 0.00140946, -2.07114e-05, 1., 0.1);
  else
    LHFun->SetParameters(1.32391,2.23676,1.02281,0.788797, -0.0594475, 0.00140946, -2.07114e-05, 1., 0.1);

  LHFun->SetParLimits(2, 0.01,2);

  LHFun->SetParLimits(4, -0.0594475*100, -0.0594475*0.7);
  LHFun->SetParLimits(5, 0.00140946*0.001, 0.00140946*20);
  LHFun->SetParLimits(6, -2.07114e-05*50, -2.07114e-05*0.02);

  LHFun->SetParLimits(7, 0.00005, 0.0005);
  //LHFun->SetParLimits(8, 0., 0.);
  LHFun->SetParLimits(8, 0.005, 0.5);

  //normalize
  //double integral = LHFun->Integral(0,100);
  //LHFun->SetParameter(0, LHFun->GetParameter(0) / integral);

  return LHFun;
}

TF1* TRDLikelihoods::prototypeLHFunctionTR()
{
  TF1* LHFun = new TF1("TRDLikelihoodTR","(landau(0)+landau(3))*expo(6)", 0., 100.);

  LHFun->SetNpx(1000);
  if (m_normalizedToLength) {
    LHFun->SetParameters(1, 0.51, 0.19, 0.3, 2.3, 0.59, 1, -0.001);
    LHFun->SetParLimits(0, 0.8, 3);
    LHFun->SetParLimits(1, 0.3, 0.6);
    LHFun->SetParLimits(3, 0.05, 0.3);
    LHFun->SetParLimits(4, 1.5, 5);
    LHFun->SetParLimits(5, 0.2, 0.8);
    LHFun->SetParLimits(6, 0.8, 1.6);
  } else {
    LHFun->SetParameters(0.533941, 2.04519, 0.90618, 0.177358, 13.3248, 4.80789, 0.312408, -0.0432751);
    LHFun->SetParLimits(0, 0.4, 1);
    LHFun->SetParLimits(1, 1, 4);
    LHFun->SetParLimits(3, 0.1, 0.4);
    LHFun->SetParLimits(4, 7, 15);
    LHFun->SetParLimits(5, 2, 8);
    LHFun->SetParLimits(6, 0.2, 0.6);
  }

  //double integral = LHFun->Integral(0,250);
  //LHFun->SetParameter(0, LHFun->GetParameter(0) / integral);

  return LHFun;
}

void TRDLikelihoods::setLHFunctionTR(const TF1* fun)
{
  if (m_LHFunctionTR)
    delete m_LHFunctionTR;
  m_LHFunctionTR = new TF1(*fun);
  saveLHs();
  qDebug("saved LHFunctionTR");
}

void TRDLikelihoods::setLHFunctionTRLayer(int i, const TF1* fun)
{
  if (m_LHFunctionTRLayer.at(i))
    delete m_LHFunctionTRLayer[i];
  m_LHFunctionTRLayer[i] = new TF1(*fun);
  saveLHs();
  qDebug("saved LHFunctionTR for layer %i", i);
}

void TRDLikelihoods::setLHFunctionTRRigLayer(double rig, int i, const TF1* fun)
{

  if (!m_LHFunctionTRRigLayer.contains(rig)) {
    QList<TF1*>& lhFunctionTRLayer = m_LHFunctionTRRigLayer[rig];
    for (int i = 0; i < 8; ++i)
      lhFunctionTRLayer << prototypeLHFunctionTR();
  }
  QList<TF1*>& lhFunctionTRLayer = m_LHFunctionTRRigLayer[rig];

  if (lhFunctionTRLayer.at(i))
    delete lhFunctionTRLayer[i];
  lhFunctionTRLayer[i] = new TF1(*fun);
  if (!m_savedTRRigs.contains(rig))
    m_savedTRRigs.insert(qLowerBound(m_savedTRRigs.begin(), m_savedTRRigs.end(), rig), rig);
  saveLHs();
  qDebug("saved LHFunctionTR for layer %i", i);
}


void TRDLikelihoods::setLHFunctionNonTR(const TF1* fun)
{
  if (m_LHFunctionNonTR)
    delete m_LHFunctionNonTR;
  m_LHFunctionNonTR = new TF1(*fun);
  saveLHs();
  qDebug("saved LHFunctionNonTR");
}

void TRDLikelihoods::setLHFunctionNonTRRig(double rig, const TF1* fun)
{
  if (m_LHFunctionNonTRRig.contains(rig))
    delete m_LHFunctionNonTRRig[rig];

  m_LHFunctionNonTRRig[rig] = new TF1(*fun);
  if (!m_savedNonTRRigs.contains(rig))
    m_savedNonTRRigs.insert(qLowerBound(m_savedNonTRRigs.begin(), m_savedNonTRRigs.end(), rig), rig);
  saveLHs();
  qDebug("saved LHFunctionNonTR");
}

void TRDLikelihoods::saveLHs()
{
  m_trdLikelihoodSettings->beginGroup("TR");
  saveFunctionParameters("default", m_LHFunctionTR);
  for (int i = 0; i < 8; ++i)
    saveFunctionParameters(QString::number(i), m_LHFunctionTRLayer.at(i));
  foreach (double rig, m_savedTRRigs) {
    m_trdLikelihoodSettings->beginGroup(QString::number(rig));
    const QList<TF1*>& lhFunctionsForRig = m_LHFunctionTRRigLayer.value(rig);
    for (int i = 0; i < 8; ++i)
      saveFunctionParameters(QString::number(i), lhFunctionsForRig.at(i));
    m_trdLikelihoodSettings->endGroup();
  }
  m_trdLikelihoodSettings->endGroup();

  m_trdLikelihoodSettings->beginGroup("nonTR");
  saveFunctionParameters("default", m_LHFunctionNonTR);
  foreach (double rig, m_savedNonTRRigs)
    saveFunctionParameters(QString::number(rig), m_LHFunctionNonTRRig.value(rig));
  m_trdLikelihoodSettings->endGroup();

  m_trdLikelihoodSettings->sync();
}

void TRDLikelihoods::loadLHs()
{
  m_trdLikelihoodSettings->beginGroup("TR");
  loadFunctionParameters(m_LHFunctionTR, "default");
  for (int i = 0; i < 8; ++i)
    loadFunctionParameters(m_LHFunctionTRLayer.at(i), QString::number(i));
  QStringList rigStringList = m_trdLikelihoodSettings->childGroups();
  qDebug() << rigStringList;
  foreach (QString rigString, rigStringList) {
    m_trdLikelihoodSettings->beginGroup(rigString);
    double rig = rigString.toDouble();
    qDebug("loading tr lh functions for %f GeV", rig);
    QList<TF1*> trLHFunctions;
    for (int i = 0; i < 8; ++i) {
      TF1* trLHFunction = prototypeLHFunctionTR();
      loadFunctionParameters(trLHFunction, QString::number(i));
      trLHFunctions << trLHFunction;
    }
    m_LHFunctionTRRigLayer.insert(rig, trLHFunctions);
    m_trdLikelihoodSettings->endGroup();
  }

  m_trdLikelihoodSettings->endGroup();

  m_trdLikelihoodSettings->beginGroup("nonTR");
  loadFunctionParameters(m_LHFunctionNonTR, "default");
  foreach (QString rigString, rigStringList) {
    double rig = rigString.toDouble();
    qDebug("loading non tr lh functions for %f GeV", rig);
    TF1* nonTRLHFunctions = prototypeLHFunctionNonTR();
    loadFunctionParameters(nonTRLHFunctions, rigString);
    m_LHFunctionNonTRRig.insert(rig, nonTRLHFunctions) ;
  }
  m_trdLikelihoodSettings->endGroup();
}

void TRDLikelihoods::saveFunctionParameters(QString name, const TF1* func)
{
  QList<QVariant> pars;
  int nPars = func->GetNpar();
  for (int i = 0; i < nPars; ++i)
    pars << func->GetParameter(i);
  m_trdLikelihoodSettings->setValue(name, pars);
}

bool TRDLikelihoods::loadFunctionParameters(TF1*func, QString name)
{
  if (!func)
    return false;
  QList<QVariant> pars = m_trdLikelihoodSettings->value(name).toList();
  if (func->GetNpar() != pars.size()) {
    qDebug("couldnt load pars");
    return false;
  }
  for(int i = 0; i < pars.size(); ++i){
    double par = pars.at(i).toDouble();
    func->SetParameter(i, par);
  }
  return true;
}

double TRDLikelihoods::savedRigValueNonTR(double rig) {
  return findNearestValueInSortedList(m_LHFunctionNonTRRig.keys(), rig);
}

double TRDLikelihoods::savedRigValueTR(double rig) {
  return findNearestValueInSortedList(m_LHFunctionTRRigLayer.keys(), rig);
}

double TRDLikelihoods::findNearestValueInSortedList(const QList<double>& lookupList, double value)
{
  //looks up nearest saved rigidity value, be careful to use correct binning when saving the known rigidities values
  //bin centers of a log binning would produce here wrong returns!!!!
  QList<double>::const_iterator it_geq= qLowerBound(lookupList, value);
  if (it_geq == lookupList.constBegin())
    return lookupList.first();
  else if (it_geq == lookupList.constEnd() || it_geq == lookupList.constEnd()-1)
    return lookupList.last();
  else
    return (value - *(it_geq-1) < *it_geq - value) ? *(it_geq-1) : *it_geq;
}
