#include "MoPVConstant.hh"

#include "PostAnalysisCanvas.hh"

#include <TH1D.h>
#include <TF1.h>

#include <QString>

MoPVConstant::MoPVConstant(QList<PostAnalysisCanvas*> canvasList)
  : PostAnalysisPlot()
  , H1DPlot()
{
  int nMoPVs = canvasList.size();
  setTitle(qPrintable("Constant MoPVs" + QString::number(nMoPVs)));
  TH1D* hist = new TH1D(qPrintable(title()), "", nMoPVs,1-0.5,nMoPVs+0.5);
  setAxisTitle("module/channel","MoPV");

  for (int i = 0; i < nMoPVs; ++i) {
    hist->GetXaxis()->SetBinLabel(i+1, qPrintable(QString::number(i+1)));

    PostAnalysisCanvas* canvas = canvasList.at(i);
    TH1D* trdHist = canvas->histograms1D().at(0);
    double mopv = 0;
    double mopvErr = 0;
    if (!fitMoPV(trdHist, mopv, mopvErr))
      continue;
    hist->SetBinContent(i+1, mopv);
    hist->SetBinError(i+1, mopvErr);
  }
  drawOption(RootPlot::E1X0);
  addHistogram(hist, RootPlot::E1X0);
}


bool MoPVConstant::fitMoPV(TH1D* hist, double& mopv, double& mopvErr)
{
  if (hist->GetEntries() < 30)
    return false;
  TF1* fit = new TF1("fitMopv","landau");

  hist->Fit(fit, "QN0R", "", 0.1, 3);

  mopv = fit->GetParameter(1);
  mopvErr = fit->GetParError(1);

  delete fit;
  return true;
}
