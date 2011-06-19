Double_t pfun(Double_t *x, Double_t *par) {
  //Double_t mpshift  = -0.22278298;
  if(x[0]<par[1]){
    //    Double_t mean= par[1] - mpshift * par[2];
    return par[0]*TMath::Landau(x[0],par[1],par[2])*TMath::Exp(par[4]*x[0]+par[5]*x[0]*x[0]+par[6]*x[0]*x[0]*x[0]);
  }
  else {
    //    Double_t mean= par[1] - mpshift * par[3];
    return par[0]*TMath::Landau(x[0],par[1],par[3])*TMath::Exp(par[4]*x[0]+par[5]*x[0]*x[0]+par[6]*x[0]*x[0]*x[0]);
  }
}


/**
  p[0] normalization factor
  p[1] mopv
  p[2] sigma of rising landau flank
  p[3] sigma of falling landau flank
  p[4] limit above which an exponential decay function is used
  p[5] exponential factor (should be negative)
**/

Double_t pfunperdaix(Double_t *x, Double_t *par) {
  if(x[0]<par[1] && x[0] < par[4]){
    return par[0]*TMath::Landau(x[0],par[1],par[2]);
  }
  else if(x[0]>=par[1] && x[0] < par[4]){
    return par[0]*TMath::Landau(x[0],par[1],par[3]);
  }else{
    return par[0]*TMath::Landau(par[4],par[1],par[3]) *exp(par[5]*(x[0]-par[4]));
  }
}
