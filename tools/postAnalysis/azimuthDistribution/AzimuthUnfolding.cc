#include "AzimuthUnfolding.hh"

#include "Corrections.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>
#include <TMultiGraph.h>
#include <TMath.h>
#include <TUnfold.h>
#include <TAxis.h>
#include <TArray.h>
#include <TGraph.h>
#include <TSpline.h>
#include <TH1F.h>
#include <TSpline.h>
//#include <TUnfoldSys.h>
#include <TError.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>
#include <QSettings>
#include <QVector>

AzimuthUnfolding::AzimuthUnfolding(TH2D* migrationHistogram, TH1D* unfoldInput)
  : m_migrationHistogram(new TH2D(*migrationHistogram))
  , m_unfoldInput(new TH1D(*unfoldInput))
  , m_unfoldedHistogram(0)
  , m_rhoIj(0)
  , m_lCurve(0)
{
  unfold();
}

AzimuthUnfolding::~AzimuthUnfolding()
{
  delete m_migrationHistogram;
  delete m_unfoldInput;
  delete m_unfoldedHistogram;
  delete m_lCurve;
}

void AzimuthUnfolding::unfold()
{
  TUnfold unfold(m_migrationHistogram,TUnfold::kHistMapOutputVert);
  const int nGen = m_migrationHistogram->GetNbinsY();
  const double xMinGen = m_migrationHistogram->GetYaxis()->GetBinLowEdge(1);
  const double xMaxGen = m_migrationHistogram->GetYaxis()->GetBinLowEdge(nGen+1);
  
//  const double biasScale = 0.0;
  
  if(unfold.SetInput(m_unfoldInput)>=10000) {
    std::cout<<"Unfolding result may be wrong\n";
  }
  
  int nScan=30;
  // use automatic L-curve scan if start with taumin = taumax = 0.0
  double tauMin=0.;
  double tauMax=0.;
  int iBest;
  TSpline *logTauX,*logTauY;
  TGraph* lCurve;
  
  iBest=unfold.ScanLcurve(nScan, tauMin, tauMax, &lCurve, &logTauX, &logTauY);
//  double tau = unfold.GetTau();
  
  m_lCurve = new TGraph(*lCurve);
  
  Double_t t[1],x[1],y[1];
  logTauX->GetKnot(iBest,t[0],x[0]);
  logTauY->GetKnot(iBest,t[0],y[0]);
  TGraph *bestLcurve=new TGraph(1,x,y);
  
  m_bestLcurve = new TGraph(*bestLcurve);
  m_bestLcurve->SetMarkerColor(kRed);
  
  std::cout<<"tau="<<unfold.GetTau()<<"\n";
  std::cout<<"chi**2="<<unfold.GetChi2A()<<"+"<<unfold.GetChi2L()
  <<" / "<<unfold.GetNdf()<<"\n";
//  std::cout<<"chi**2(sys)="<<unfold.GetChi2Sys()<<"\n"; //ToDo: TUnfoldSys for systematic errors
  
  int *binMap = new int[nGen+2];
  for(int i = 1;i <= nGen;i++) {
    binMap[i] = i;
  }
  binMap[0] = -1; // discarde underflow bin (here: the background normalisation)
  binMap[nGen+1] = -1; // discarde overflow bin (here: bins with mass>10)
  
  const QString title = "unfolded";
  const QString xTitle = m_unfoldInput->GetXaxis()->GetTitle();
  
  m_unfoldedHistogram = new TH1D(qPrintable(title), qPrintable(xTitle),nGen,xMinGen,xMaxGen);
  m_unfoldedHistogram->Sumw2();
  m_unfoldedHistogram->SetMarkerColor(kRed);
  m_unfoldedHistogram->SetLineColor(kRed);
  
//  tau = exp(-0.474793);
//  unfold.DoUnfold(tau,m_unfoldInput,biasScale);
  
  unfold.GetOutput(m_unfoldedHistogram, binMap);
  
  m_rhoIj = unfold.GetRhoIJ("","correlation");
  
  delete[] binMap;
  binMap=0;
}