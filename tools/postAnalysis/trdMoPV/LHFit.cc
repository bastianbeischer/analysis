#include "LHFit.hh"

#include "PostAnalysisCanvas.hh"
#include "H1DPlot.hh"

#include <TH1D.h>
#include <TROOT.h>

#include <iostream>

#include <QString>
#include <QList>
#include <QDebug>


LHFit::LHFit(PostAnalysisCanvas* canvas)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
{
  gROOT->cd();
  setTitle("TRD Spectra");
  setAxisTitle("blaaablaa energy Deposition per length / (keV / mm)","#");

  setDrawOption(H1DPlot::BLANK);

  if(canvas != 0){
   TH1D* histClone = (TH1D*) canvas->histograms1D().at(0)->Clone();
   histClone->Sumw2();
   histClone->SetStats(kFALSE);
   histClone->Scale(1./histClone->Integral("width"));
   histClone->SetMarkerStyle(0);
   addHistogram(histClone, H1DPlot::BLANK);
  }
  histogram()->GetYaxis()->SetRangeUser(10e-6, 1.1*histogram()->GetMaximum());

}
