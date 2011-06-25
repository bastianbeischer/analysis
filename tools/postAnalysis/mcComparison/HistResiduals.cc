#include "HistResiduals.hh"

#include "PostAnalysisCanvas.hh"
#include "H1DPlot.hh"
#include "GraphPlot.hh"

#include <TH1D.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TROOT.h>

#include <iostream>
#include <math.h>

#include <QString>
#include <QDebug>


HistResiduals::HistResiduals(PostAnalysisCanvas* dataC, PostAnalysisCanvas* mcC)
  : QObject()
  , PostAnalysisPlot()
  , GraphPlot()
{
  gROOT->cd();
  setTitle("TRD Spectra Residuals");
  setAxisTitle("energy Deposition / keV","pull");

  TH1D* dataHist = dataC->histograms1D().at(0);
  TH1D* mcHist = mcC->histograms1D().at(0);

  TH1D* dataHistClone = (TH1D*) dataHist->Clone();
  dataHistClone->SetName("testbeam");
  TH1D* mcHistClone = (TH1D*) mcHist->Clone();
  mcHistClone->SetName("perdaix mc");

  dataHistClone->Sumw2();
  dataHistClone->Scale(1. / dataHistClone->Integral("width"));

  mcHistClone->Sumw2();
  mcHistClone->Scale(1. / mcHistClone->Integral("width"));

  QVector<double> xValues, xErrors, yValues, yErrors;

  for( int i = 1; i < mcHistClone->GetNbinsX()-1; i++)
  {
    double sigma = sqrt(dataHistClone->GetBinError(i)*dataHistClone->GetBinError(i) + mcHistClone->GetBinError(i)*mcHistClone->GetBinError(i));
    if (!(sigma > 0))
      continue;
    xValues << dataHistClone->GetBinCenter(i);
    xErrors << 0.;
    yValues << (dataHistClone->GetBinContent(i) - mcHistClone->GetBinContent(i)) / sigma;
    yErrors << 0.;
  }

  TGraphErrors* res_mcdata = new TGraphErrors(xValues.size(), &*xValues.begin(), &*yValues.begin());
  res_mcdata->SetLineColor(kRed);
  res_mcdata->SetMarkerStyle(24);

  addGraph(res_mcdata,"P");
}
