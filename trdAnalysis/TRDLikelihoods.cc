#include "TRDLikelihoods.hh"

#include <TF1.h>
#include <TMath.h>

TRDLikelihoods::TRDLikelihoods()
{
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

  TF1* LHFun = new TF1("TRDLikelihoodNonTR",TRDLikelihoods::functionNonTR,0.,100,6);

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

Double_t TRDLikelihoods::functionNonTR(Double_t *x, Double_t *par)
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
