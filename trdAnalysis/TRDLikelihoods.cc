#include "TRDLikelihoods.hh"

#include <QMutex>
#include <QMutexLocker>
#include <QSettings>

#include <TF1.h>
#include <TMath.h>

double functionNonTR(double *x, double *par)
{
  /**
    p[0] normalization factor
    p[1] mopv
    p[2] sigma of rising landau flank
    p[3] sigma of falling landau flank
    p[4] limit above which an exponential decay function is used
    p[5] exponential factor (should be negative)
  **/
  if(x[0]<par[1] && x[0] < par[4]){
    return par[0]*TMath::Landau(x[0],par[1],par[2]);
  }
  else if(x[0]>=par[1] && x[0] < par[4]){
    return par[0]*TMath::Landau(x[0],par[1],par[3]);
  }else{
    return par[0]*TMath::Landau(par[4],par[1],par[3]) *exp(par[5]*(x[0]-par[4]));
  }
}



TRDLikelihoods* TRDLikelihoods::m_instance = 0;
QMutex TRDLikelihoods::m_mutex;

TRDLikelihoods* TRDLikelihoods::instance()
{
  if (!m_instance) {
    QMutexLocker locker(&m_mutex);
    if (!m_instance)
      m_instance = new TRDLikelihoods();
  }
  return m_instance;
}

TRDLikelihoods::TRDLikelihoods()
  : m_LHFunctionTR(getPrototypeLHFunctionTR())
  , m_LHFunctionNonTR(getPrototypeLHFunctionNonTR())
{
  for (int i = 0; i < 8; ++i)
    m_LHFunctionTRLayer << getPrototypeLHFunctionTR();

  const char* env = getenv("PERDAIXANA_PATH");
  if (env == 0)
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  QString path(env);
  path += "/conf/";
  m_trdLikelihoodSettings = new QSettings(path + "TRDLikelihood.conf", QSettings::IniFormat);

  loadLHs();
}

TF1* TRDLikelihoods::getPrototypeLHFunctionNonTR()
{
  /**
    p[0] normalization factor
    p[1] mopv
    p[2] sigma of rising landau flank
    p[3] sigma of falling landau flank
    p[4] limit above which an exponential decay function is used
    p[5] exponential factor (should be negative)
  **/

  TF1* LHFun = new TF1("TRDLikelihoodNonTR", functionNonTR, 0., 100., 6);

  //set default values:
  LHFun->SetNpx(1000);
  LHFun->SetParameters(7.5, 0.34, 0.115, 0.135, 7, -0.1);
  for (int i = 4; i < 5; i++)
    LHFun->SetParLimits(i,LHFun->GetParameter(i),LHFun->GetParameter(i));
  LHFun->SetParLimits(5,-0.3,-0.01);

  //normalize
  //double integral = LHFun->Integral(0,100);
  //LHFun->SetParameter(0, LHFun->GetParameter(0) / integral);

  return LHFun;
}

TF1* TRDLikelihoods::getPrototypeLHFunctionTR()
{
  TF1* LHFun = new TF1("TRDLikelihoodTR","(landau(0)+landau(3))*expo(6)", 0., 100.);

  LHFun->SetNpx(1000);
  LHFun->SetParameters(1, 0.51, 0.19, 0.3, 2.3, 0.59, 1, -0.001);
  LHFun->SetParLimits(0, 0.8, 1.4);
  LHFun->SetParLimits(1, 0.3, 0.6);
  LHFun->SetParLimits(3, 0.2, 0.6);
  LHFun->SetParLimits(4, 2, 4);
  LHFun->SetParLimits(5, 0.4, 1.5);
  LHFun->SetParLimits(6, 0.8, 1.4);

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


void TRDLikelihoods::setLHFunctionNonTR(const TF1* fun)
{
  if (m_LHFunctionNonTR)
    delete m_LHFunctionNonTR;
  m_LHFunctionNonTR = new TF1(*fun);
  saveLHs();
  qDebug("saved LHFunctionNonTR");
}

void TRDLikelihoods::saveLHs()
{
  m_trdLikelihoodSettings->beginGroup("TR");
  saveFunctionParameters("default", m_LHFunctionTR);
  for (int i = 0; i < 8; ++i)
    saveFunctionParameters(QString::number(i), m_LHFunctionTRLayer.at(i));
  m_trdLikelihoodSettings->endGroup();

  m_trdLikelihoodSettings->beginGroup("nonTR");
  saveFunctionParameters("default", m_LHFunctionNonTR);
  m_trdLikelihoodSettings->endGroup();

  m_trdLikelihoodSettings->sync();
}

void TRDLikelihoods::loadLHs()
{
  m_trdLikelihoodSettings->beginGroup("TR");
  loadFunctionParameters(m_LHFunctionTR, "default");
  for (int i = 0; i < 8; ++i)
    loadFunctionParameters(m_LHFunctionTRLayer.at(i), QString::number(i));
  m_trdLikelihoodSettings->endGroup();

  m_trdLikelihoodSettings->beginGroup("nonTR");
  loadFunctionParameters(m_LHFunctionNonTR, "default");
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


