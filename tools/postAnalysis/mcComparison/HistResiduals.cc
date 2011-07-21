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


HistResiduals::HistResiduals(PostAnalysisCanvas* canvas1, PostAnalysisCanvas* canvas2, bool usePull)
  : QObject()
  , PostAnalysisPlot()
  , GraphPlot()
{
  gROOT->cd();

  TH1D* hist1Clone = (TH1D*) canvas1->histograms1D().at(0)->Clone();
  TH1D* hist2Clone = (TH1D*) canvas2->histograms1D().at(0)->Clone();

  if (usePull) {
    setTitle("TRD Spectra Pull");
    setAxisTitle(hist1Clone->GetXaxis()->GetTitle(),"pull");
  } else {
    setTitle("TRD Spectra Residuals");
    setAxisTitle(hist1Clone->GetXaxis()->GetTitle(),"residual");
  }

  double lowerIntegralBin = hist1Clone->FindBin(0.01 * hist1Clone->GetXaxis()->GetXmax());
  double upperIntegralBin = hist1Clone->FindBin(0.4 * hist1Clone->GetXaxis()->GetXmax());

  hist1Clone->Sumw2();
  hist1Clone->Scale(1. / hist1Clone->Integral(lowerIntegralBin, upperIntegralBin, "width"));

  hist2Clone->Sumw2();
  hist2Clone->Scale(1. / hist2Clone->Integral(lowerIntegralBin, upperIntegralBin, "width"));

  QVector<double> xValues, xErrors, yValues, yErrors;

  for( int i = 1; i < hist1Clone->GetNbinsX()-1; i++)
  {
    double sigma = sqrt(hist1Clone->GetBinError(i)*hist1Clone->GetBinError(i) + hist2Clone->GetBinError(i)*hist2Clone->GetBinError(i));
    if (!(sigma > 0))
      continue;
    xValues << hist1Clone->GetBinCenter(i);
    xErrors << 0.;
    if (usePull) {
      yValues << (hist1Clone->GetBinContent(i) - hist2Clone->GetBinContent(i)) / sigma;
      yErrors << 0.;
    } else {
      yValues << (hist1Clone->GetBinContent(i) - hist2Clone->GetBinContent(i));
      yErrors << sigma;
    }
  }

  TGraphErrors* graph= new TGraphErrors(xValues.size(), &*xValues.begin(), &*yValues.begin(), &*xErrors.begin(), &*yErrors.begin());
  if (usePull)
    graph->SetMarkerStyle(24);
  else
    graph->SetMarkerStyle(0);

  addGraph(graph, P);
}
