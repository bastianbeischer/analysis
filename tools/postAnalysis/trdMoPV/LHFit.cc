#include "LHFit.hh"

#include "PostAnalysisCanvas.hh"
#include "H1DPlot.hh"
#include "TRDLikelihoods.hh"

#include <TH1D.h>
#include <TF1.h>
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
  setDrawOption(H1DPlot::BLANK);

  if (canvas != 0) {
    TH1D* histClone = (TH1D*) canvas->histograms1D().at(0)->Clone();
    histClone->Sumw2();
    histClone->SetStats(kFALSE);
    histClone->Scale(1./histClone->Integral("width"));
    histClone->SetMarkerStyle(0);
    addHistogram(histClone, H1DPlot::BLANK);
  }
  histogram()->GetYaxis()->SetRangeUser(10e-6, 1.1*histogram()->GetMaximum());

  setTitle(canvas->name());
  setAxisTitle(histogram()->GetXaxis()->GetTitle(),histogram()->GetYaxis()->GetTitle());

  TRDLikelihoods* trdLHs = TRDLikelihoods::instance();
  TF1* lhFunNonTR = new TF1(*trdLHs->getLHFunctionNonTR());
  TF1* lhFunTR = new TF1(*trdLHs->getLHFunctionTR());
  addFunction(lhFunNonTR);
  addFunction(lhFunTR);
}
