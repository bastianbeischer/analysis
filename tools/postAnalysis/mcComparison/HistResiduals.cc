#include "HistResiduals.hh"

#include "PostAnalysisCanvas.hh"
#include "H1DPlot.hh"
#include "GraphPlot.hh"

#include <TH1D.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TROOT.h>

#include <iostream>

#include <QString>
#include <QDebug>


HistResiduals::HistResiduals(PostAnalysisCanvas* dataC, PostAnalysisCanvas* mcC)
  : QObject()
  , PostAnalysisPlot()
  , GraphPlot()
{
  gROOT->cd();
  setTitle("TRD Spectra Residuals");
  setAxisTitle("energy Deposition / keV","#-difference");

  TH1D* dataHist = dataC->histograms1D().at(0);
  TH1D* mcHist = mcC->histograms1D().at(0);

  TH1D* dataHistClone = (TH1D*) dataHist->Clone();
  dataHistClone->SetName("testbeam");
  TH1D* mcHistClone = (TH1D*) mcHist->Clone();
  mcHistClone->SetName("perdaix mc");

  dataHistClone->Sumw2();
  dataHistClone->Scale(1. / dataHistClone->GetEntries());

  mcHistClone->Sumw2();
  mcHistClone->Scale(1. / mcHistClone->GetEntries());

  TH1D* res_hist = new TH1D("res_mcdata_hist", "res_mcdata_hist", mcHistClone->GetNbinsX(), mcHistClone->GetXaxis()->GetXmin(), mcHistClone->GetXaxis()->GetXmax());

  for( int i = 0; i < mcHistClone->GetNbinsX(); i++)
  {
    res_hist->SetBinContent(i, dataHistClone->GetBinContent(i) - mcHistClone->GetBinContent(i));
    res_hist->SetBinError(i,dataHistClone->GetBinError(i));
  }

  TGraphErrors* res_mcdata = new TGraphErrors(res_hist);
  res_mcdata->SetLineColor(kRed);
  res_mcdata->SetMarkerStyle(24);

  addGraph(res_mcdata,"P");
}
