#include "RigidityUnfolding.hh"

#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TUnfold.h>
#include <TArray.h>
#include <TGraph.h>
#include <TSpline.h>

#include <cmath>

#include <QDebug>

RigidityUnfolding::RigidityUnfolding(TH2D* migrationHistogram, TH1D* unfoldInput)
: m_migrationHistogram(new TH2D(*migrationHistogram))
, m_unfoldInput(new TH1D(*unfoldInput))
, m_unfoldedHistogram(0)
, m_rhoIj(0)
, m_lCurve(0)
{
  unfold();
}

RigidityUnfolding::~RigidityUnfolding()
{
  delete m_migrationHistogram;
  delete m_unfoldInput;
  delete m_unfoldedHistogram;
  delete m_lCurve;
}

void RigidityUnfolding::unfold()
{
  TUnfold unfold(m_migrationHistogram, TUnfold::kHistMapOutputVert);
  const int nGen = m_migrationHistogram->GetNbinsY();
  const double xMinGen = m_migrationHistogram->GetYaxis()->GetBinLowEdge(1);
  const double xMaxGen = m_migrationHistogram->GetYaxis()->GetBinLowEdge(nGen+1);
  
  if (unfold.SetInput(m_unfoldInput) >= 10000)
    qDebug() << "Unfolding result may be wrong";
  
  int nScan = 70;
  // use automatic L-curve scan if start with taumin = taumax = 0.0
  double tauMin = 1e-7;
  double tauMax = 1.;
  int iBest;
  TSpline *logTauX = 0, *logTauY = 0;
  TGraph* lCurve = 0;
  
  iBest = unfold.ScanLcurve(nScan, tauMin, tauMax, &lCurve, &logTauX, &logTauY);
  m_lCurve = new TGraph(*lCurve);
  
  double t = 0, x = 0, y = 0;
  logTauX->GetKnot(iBest, t, x);
  logTauY->GetKnot(iBest, t, y);
  TGraph* bestLcurve = new TGraph(1, &x, &y);
  
  m_bestLcurve = new TGraph(*bestLcurve);
  m_bestLcurve->SetMarkerColor(kRed);
  
  qDebug() << "tau =" << unfold.GetTau();
  qDebug() << "chi2 =" << unfold.GetChi2A() << "+" << unfold.GetChi2L() << "/" << unfold.GetNdf();
  
  int* binMap = new int[nGen+2];
  for (int i = 1; i <= nGen; ++i)
    binMap[i] = i;
  binMap[0] = -1; // discarde underflow bin
  binMap[nGen+1] = -1; // discarde overflow bin
  
  const QString& title = "unfolded";
  const QString& xTitle = m_unfoldInput->GetXaxis()->GetTitle();
  
  m_unfoldedHistogram = new TH1D(qPrintable(title), qPrintable(xTitle), nGen, xMinGen, xMaxGen);
  m_unfoldedHistogram->Sumw2();
  m_unfoldedHistogram->SetMarkerColor(kRed);
  m_unfoldedHistogram->SetLineColor(kRed);
  
  unfold.GetOutput(m_unfoldedHistogram/*, binMap*/);
  
  m_rhoIj = unfold.GetRhoIJ("", "correlation");
  
  delete[] binMap;
  binMap = 0;
}





//TUnfold unfold(hMigration,TUnfold::kHistMapOutputVert);
//
//unfold.SetInput(hPowerSpectrumRec);
//
//Int_t nScan=30;
//Int_t iBest;
//TSpline *logTauX,*logTauY;
//TGraph *lCurve;
//
//iBest=unfold.ScanLcurve(nScan,1.E-7,1.,&lCurve);
//
//std::cout<<"tau="<<unfold.GetTau()<<"\n";
//
//TH1D *x=new TH1D("hUnfolded","hUnfolded",nBinlogGen,&xMinGen[0]);
//unfold.GetOutput(x);
//TH2D *rhoij=unfold.GetRhoIJ("correlation","myVariable");
//
//x->SetLineColor(kBlue);
//x->SetLineWidth(3);
//
//hPowerSpectrumGen->Sumw2();
//hPowerSpectrumGen->Divide(hNormGen);
//hPowerSpectrumGen->Draw("hist.E");
//hPowerSpectrumGen->Fit("FunPowerSpectrum","E");
//hPowerSpectrumGen->GetXaxis()->SetTitle("P [GeV]");
//hPowerSpectrumGen->GetYaxis()->SetTitle("N/GeV");
//
//hPowerSpectrumRec->Sumw2();
//hPowerSpectrumRec->Divide(hNormRec);
//hPowerSpectrumRec->SetLineColor(kRed);
//hPowerSpectrumRec->Draw("same.hist.E");
//
//x->Divide(hNormGen);
//x->Draw("same");
//
//TCanvas* c1 = new TCanvas();
//lCurve->SetMarkerStyle(20);
//lCurve->SetMarkerSize(1);
//lCurve->Draw("AP");
//
//TCanvas *cSSC3 = new TCanvas("cSSC3","SSC3",100,0,500,500);
//cSSC3->SetLogx();
//cSSC3->SetLogy();
//cSSC3->SetLeftMargin(0.15);
//cSSC3->Draw();
//
//hMigration->Sumw2();
//hMigration->Divide(hNorm2);
//
//hMigration->Draw("col.z");
//hMigration->GetXaxis()->SetTitle("Prec [GeV]");
//hMigration->GetYaxis()->SetTitle("Pgen [GeV]");

