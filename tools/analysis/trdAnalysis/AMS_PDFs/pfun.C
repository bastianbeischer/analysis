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
