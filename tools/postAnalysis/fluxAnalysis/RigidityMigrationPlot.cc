#include "RigidityMigrationPlot.hh"

#include "PostAnalysisCanvas.hh"

#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>

RigidityMigrationPlot::RigidityMigrationPlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H2DPlot()
{
  TH2D* histogram = new TH2D(*canvas->histograms2D().at(0));
  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);
  histogram->SetTitle(qPrintable(title.remove(" histogram")));
  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle(), histogram->GetZaxis()->GetTitle());
}

RigidityMigrationPlot::~RigidityMigrationPlot()
{
}

TH2D* RigidityMigrationPlot::createMigrationHistogramNormalizedToArea()
{
  TH2D* normalizedHistogram = new TH2D(*histogram());
  const QString& newTitle = QString(normalizedHistogram->GetTitle()) + "migration normalized to area";
  normalizedHistogram->SetTitle(qPrintable(newTitle));
  for(int i = 0; i < normalizedHistogram->GetNbinsX(); i++) {
    for(int j = 0; j < normalizedHistogram->GetNbinsY(); j++) {
      double binContent = normalizedHistogram->GetBinContent(i + 1, j + 1);
      double binArea = normalizedHistogram->GetYaxis()->GetBinWidth(j+1) * normalizedHistogram->GetXaxis()->GetBinWidth(i+1);
      normalizedHistogram->SetBinContent(i+1, j+1, binContent / binArea);
      normalizedHistogram->SetBinError(i+1, j+1, binContent / binArea);
    }
  }
  normalizedHistogram->GetZaxis()->SetTitle("entries / GeV^{2}");
  return normalizedHistogram;
}

TH1D* RigidityMigrationPlot::createRecordedSpectrum()
{
  const int nBins = histogram()->GetNbinsY();
  double nBinsNew;
  double nBinsStart;
  if (nBins%2 == 0)
    nBinsNew = nBins / 2;
  else
    nBinsNew = (nBins - 1) / 2;
  if (nBins%2 == 0)
    nBinsStart = nBinsNew + 1;
  else
    nBinsStart = nBinsNew + 2;
  TH1D* newHistogram = projectionX(histogram(), nBinsStart, nBins);
  newHistogram->SetTitle("MC recorded spectrum");
  newHistogram->SetMarkerColor(kBlue);
  newHistogram->SetLineColor(kBlue);
  newHistogram->GetXaxis()->SetTitle("rigidity / GeV");
  newHistogram->GetXaxis()->SetTitle("entries");
  TH1D* negativeSide = projectionX(histogram(), 1, nBinsNew);
  newHistogram->Add(negativeSide);
  delete negativeSide;
  negativeSide = 0;
  return newHistogram;
}

TH1D* RigidityMigrationPlot::createGeneratedSpectrum()
{
  const int nBins = histogram()->GetNbinsX();
  double nBinsNew;
  double nBinsStart;
  if (nBins%2 == 0)
    nBinsNew = nBins / 2;
  else
    nBinsNew = (nBins - 1) / 2;
  if (nBins%2 == 0)
    nBinsStart = nBinsNew + 1;
  else
    nBinsStart = nBinsNew + 2;
  TH1D* newHistogram = projectionY(histogram(), nBinsStart, nBins);
  newHistogram->SetTitle("MC generated spectrum");
  newHistogram->SetMarkerColor(kRed);
  newHistogram->SetLineColor(kRed);
  newHistogram->GetXaxis()->SetTitle("rigidity / GeV");
  newHistogram->GetYaxis()->SetTitle("entries");
  TH1D* negativeSide = projectionY(histogram(), 1, nBinsNew);
  newHistogram->Add(negativeSide);
  delete negativeSide;
  negativeSide = 0;
  return newHistogram;
}

TH1D* RigidityMigrationPlot::projectionX(TH2D* histogram, int firstBin, int lastBin)
{
  QString projectionName = QString(histogram->GetName()) + "_projX_bin_" + QString::number(firstBin) + "_to_" + QString::number(lastBin);
  TH1D* projHistogram = histogram->ProjectionX(qPrintable(projectionName), firstBin, lastBin);
  if (!projHistogram->GetSumw2())
    projHistogram->Sumw2();
  return projHistogram;
}

TH1D* RigidityMigrationPlot::projectionY(TH2D* histogram, int firstBin, int lastBin)
{
  QString projectionName = QString(histogram->GetName()) + "_projY_bin_" + QString::number(firstBin) + "_to_" + QString::number(lastBin);
  TH1D* projHistogram = histogram->ProjectionY(qPrintable(projectionName), firstBin, lastBin);
  if (!projHistogram->GetSumw2())
    projHistogram->Sumw2();
  return projHistogram;
}
