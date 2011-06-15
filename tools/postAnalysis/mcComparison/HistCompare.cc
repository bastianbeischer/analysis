#include "HistCompare.hh"

#include "PostAnalysisCanvas.hh"
#include "H1DPlot.hh"

#include <TH1D.h>

#include <QString>
#include <QDebug>


HistCompare::HistCompare(PostAnalysisCanvas* dataC, PostAnalysisCanvas* mcC)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
{
  setTitle("TRD Spectra");
  setAxisTitle("energy Deposition / keV","#");
  TH1D* dataHist = dataC->histograms1D().at(0);
  TH1D* mcHist = mcC->histograms1D().at(0);

  TH1D* dataHistClone = (TH1D*)dataHist->Clone();
  dataHistClone->SetName("testbeam");
  TH1D* mcHistClone = (TH1D*)mcHist->Clone();
  mcHistClone->SetName("perdaix mc");

  //dataHistClone->Sumw2();
  dataHistClone->Scale(1. / dataHistClone->GetEntries());
  //mcHistClone->Sumw2();
  mcHistClone->Scale(1. / mcHistClone->GetEntries());

  dataHist->GetXaxis()->SetRangeUser(0, dataHist->GetMaximum()*1.1);
  mcHistClone->SetLineColor(kRed);

  addHistogram(dataHistClone);
  addHistogram(mcHistClone);

}
