#include "HistCompare.hh"

#include "PostAnalysisCanvas.hh"
#include "H1DPlot.hh"

#include <TH1D.h>
#include <TROOT.h>

#include <iostream>

#include <QString>
#include <QList>
#include <QDebug>


HistCompare::HistCompare(QList<PostAnalysisCanvas*> canvases)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
{
  gROOT->cd();
  setTitle(canvases.at(0)->name());
  setAxisTitle(canvases.at(0)->histograms1D().at(0)->GetXaxis()->GetTitle(), canvases.at(0)->histograms1D().at(0)->GetYaxis()->GetTitle());

  setDrawOption(H1DPlot::BLANK);

  double ymax = 0;
  int nColour = 1;
  foreach (PostAnalysisCanvas* canvas, canvases) {
    if(canvas != 0){
     TH1D* histClone = (TH1D*) canvas->histograms1D().at(0)->Clone();
     histClone->Sumw2();
     histClone->SetStats(kFALSE);
     //double lowerIntegralBin = histClone->FindBin(0.01 * histClone->GetXaxis()->GetXmax());
     //double upperIntegralBin = histClone->FindBin(0.4 * histClone->GetXaxis()->GetXmax());
     //histClone->Scale(1./histClone->Integral(lowerIntegralBin, upperIntegralBin, "width"));
     histClone->Scale(1./histClone->Integral("width"));
     histClone->SetLineColor(nColour++);

     ymax = qMax(ymax, histClone->GetMaximum());

     histClone->SetMarkerStyle(0);
     addHistogram(histClone, H1DPlot::BLANK);
    }
  }
  histogram()->GetYaxis()->SetRangeUser(1e-4, ymax*1.1);
}
