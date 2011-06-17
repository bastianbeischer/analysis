#include "HistCompare.hh"

#include "PostAnalysisCanvas.hh"
#include "H1DPlot.hh"

#include <TH1D.h>
#include <TROOT.h>

#include <iostream>

#include <QString>
#include <QList>
#include <QDebug>


HistCompare::HistCompare(PostAnalysisCanvas* dataC, PostAnalysisCanvas* mcC, PostAnalysisCanvas* oldmcC)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
{
  gROOT->cd();
  setTitle("TRD Spectra");
  setAxisTitle("energy Deposition / keV","#");
  QList<PostAnalysisCanvas*> postCan;
  postCan.append(dataC);
  postCan.append(mcC);
  postCan.append(oldmcC);

  QList<TH1D*> histos;
  for(int i = 0; i < postCan.count(); i++)
  {
    if(postCan.at(i) != 0){
      histos.append(postCan.at(i)->histograms1D().at(0));
    }
  }

  TH1D* hist_clone[histos.count()];
  for(int i = 0; i < histos.count(); i++){
    char title[128];
    sprintf(title, "hist_clone%d", i);
    hist_clone[i] = (TH1D*) histos.at(i)->Clone();
    hist_clone[i]->Scale(1./hist_clone[i]->GetEntries());
    hist_clone[i]->SetLineColor(i+1);
    hist_clone[i]->GetYaxis()->SetRangeUser(0,0.06);

    addHistogram(hist_clone[i]);
  }
}
