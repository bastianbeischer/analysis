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
  const char* env = getenv("PERDAIXANA_PATH");
  if (env == 0)
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  QString path(env);
  path += "/conf/";
  m_trdLikelihoodSettings = new QSettings(path + "TRDLikelihood.conf", QSettings::IniFormat);

  loadLHs();
}

TF1* TRDLikelihoods::getPrototypeLHFunctionTR()
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
  LHFun->SetParameters(4.32391,2.23676,1.02281,0.788797,6,-0.1);
  for (int i = 4; i < 5; i++)
    LHFun->SetParLimits(i,LHFun->GetParameter(i),LHFun->GetParameter(i));
  LHFun->SetParLimits(5,-0.3,-0.01);

  //normalize
  double integral = LHFun->Integral(0,100);
  LHFun->SetParameter(0, LHFun->GetParameter(0) / integral);

  return LHFun;
}

TF1* TRDLikelihoods::getPrototypeLHFunctionNonTR()
{
  TF1* LHFun = new TF1("TRDLikelihoodTR","(landau(0)+landau(3))");

  LHFun->SetNpx(1000);
  LHFun->SetParameters(0.533941,3,0.90618,0.177358,7,4.80789);
  LHFun->SetParLimits(1,2,3.5);
  LHFun->SetParLimits(4,6.5,9);
  LHFun->SetParLimits(5,0.1,2);

  double integral = LHFun->Integral(0,250);
  LHFun->SetParameter(0, LHFun->GetParameter(0) / integral);

  return LHFun;
}

void TRDLikelihoods::saveLHs()
{
  m_trdLikelihoodSettings->beginGroup("TR");
  saveFunctionParameters("default", m_LHFunctionTR);
  m_trdLikelihoodSettings->endGroup();

  m_trdLikelihoodSettings->beginGroup("nonTR");
  saveFunctionParameters("default", m_LHFunctionNonTR);
  m_trdLikelihoodSettings->endGroup();
}

void TRDLikelihoods::loadLHs()
{
  m_trdLikelihoodSettings->beginGroup("TR");
  loadFunctionParameters(m_LHFunctionTR, "default");
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


