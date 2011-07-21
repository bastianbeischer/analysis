#include "TRDLikelihoods.hh"
#include "TRDReconstruction.hh"

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
    p[4] linear term in exponential
    p[5] quadratic term in exponential
    p[6] cubic term in exponential
  **/
  if (x[0]<par[1])
    return par[0]*TMath::Landau(x[0],par[1],par[2])*TMath::Exp(par[4]*x[0]+par[5]*x[0]*x[0]+par[6]*x[0]*x[0]*x[0]);
  else
    return par[0]*TMath::Landau(x[0],par[1],par[3])*TMath::Exp(par[4]*x[0]+par[5]*x[0]*x[0]+par[6]*x[0]*x[0]*x[0]);
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
  : m_normalizedToLength(TRDReconstruction::calculateLengthInTube)
  , m_LHFunctionTR(prototypeLHFunctionTR())
  , m_LHFunctionNonTR(prototypeLHFunctionNonTR())
{
  for (int i = 0; i < 8; ++i)
    m_LHFunctionTRLayer << prototypeLHFunctionTR();

  const char* env = getenv("PERDAIXANA_PATH");
  if (env == 0)
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  QString path(env);
  path += "/conf/";
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
  **/

  TF1* LHFun = new TF1("TRDLikelihoodNonTR", functionNonTR, 0., 100., 7);
  //set default values:
  LHFun->SetNpx(1000);
  if (m_normalizedToLength)
    LHFun->SetParameters(7.5, 0.34, 0.115, 0.135, -0.0594475, 0.00140946, -2.07114e-05);
  else
    LHFun->SetParameters(1.32391,2.23676,1.02281,0.788797, -0.0594475, 0.00140946, -2.07114e-05);

  LHFun->SetParLimits(4, -0.0594475*10, -0.0594475*0.1);
  LHFun->SetParLimits(5, 0.00140946*0.1, 0.00140946*20);
  LHFun->SetParLimits(6, -2.07114e-05*20, -2.07114e-05*0.02);

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
    LHFun->SetParLimits(0, 0.8, 1.4);
    LHFun->SetParLimits(1, 0.3, 0.6);
    LHFun->SetParLimits(3, 0.2, 0.6);
    LHFun->SetParLimits(4, 2, 5);
    LHFun->SetParLimits(5, 0.3, 0.65);
    LHFun->SetParLimits(6, 0.8, 1.4);
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


