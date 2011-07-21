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

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>
#include <QSettings>
#include <QVector>

AzimuthUnfolding::AzimuthUnfolding(TH2D* migrationHistogram, TH1D* histogramToUnfold)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_migrationHistogram(new TH2D(*migrationHistogram))
  , m_histogramToUnfold(new TH1D(*histogramToUnfold))
  , m_rawUnfoldedHistogram(0)
  , m_unfoldedHistogram(0)
  , m_rhoIj(0)
  , m_lCurve(0)
{
  unfold();
  
  addHistogram(m_rawUnfoldedHistogram);
  setAxisTitle(m_rawUnfoldedHistogram->GetXaxis()->GetTitle(), m_rawUnfoldedHistogram->GetYaxis()->GetTitle());
  setTitle(m_rawUnfoldedHistogram->GetTitle());
}

AzimuthUnfolding::~AzimuthUnfolding()
{
  delete m_migrationHistogram;
  delete m_histogramToUnfold;
  delete m_unfoldedHistogram;
}

void AzimuthUnfolding::unfold()
{
  TUnfold unfold(m_migrationHistogram,TUnfold::kHistMapOutputVert);
  double biasScale = 0.0;
  
  unfold.SetInput(m_histogramToUnfold);
  
  int nScan=30;
  int iBest;
//  TSpline *logTauX,*logTauY;
  TGraph* lCurve;
  
  iBest=unfold.ScanLcurve(nScan,0.0,0.0,&lCurve);
  double tau = unfold.GetTau();
  m_lCurve = lCurve;
  
  std::cout<<"tau="<<tau<<"\n";
  
  unfold.DoUnfold(tau,m_histogramToUnfold,biasScale);
  m_rawUnfoldedHistogram = unfold.GetOutput("","unfolded");
  m_rhoIj = unfold.GetRhoIJ("","correlation");
  
  
  int nBins = m_migrationHistogram->GetNbinsY();
  double min = m_migrationHistogram->GetYaxis()->GetBinLowEdge(1);
  double max = m_migrationHistogram->GetYaxis()->GetBinLowEdge(nBins+1);
  
  QString hTitle = "unfolded histo";
  setTitle(hTitle);
  TH1D* h = new TH1D(qPrintable(hTitle), "", nBins, min, max);
  h->Sumw2();
  
  for (int i = 0; i < nBins; ++i) {
    int bin = i+1;
    double content = m_rawUnfoldedHistogram->GetBinContent(bin);
    double contentError = m_rawUnfoldedHistogram->GetBinError(bin);
    h->SetBinContent(bin, content);
    h->SetBinError(bin, contentError);
  }
  m_unfoldedHistogram = h;
  m_unfoldedHistogram->SetTitle(qPrintable(hTitle));
  
}