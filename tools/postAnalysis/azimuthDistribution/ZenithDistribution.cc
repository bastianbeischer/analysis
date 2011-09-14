#include "ZenithDistribution.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TAxis.h>
#include <TFile.h>
#include <TROOT.h>

#include <cmath>

#include <QDebug>

ZenithDistribution::ZenithDistribution(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_zenithAcceptance(0)
{
  TH1D* histogram = new TH1D(*canvas->histograms1D().at(0));

  double integral = histogram->Integral("width");
  histogram->Scale(1/integral);

  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);

  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), "propability");

  if (readFile())
    addHistogram(m_zenithAcceptance);

  int low = histogram->GetXaxis()->FindBin(0.8);
  int up = histogram->GetXaxis()->FindBin(1);
  histogram->GetXaxis()->SetRange(low, up);
  histogram->GetYaxis()->SetTitleOffset(2);
}

ZenithDistribution::~ZenithDistribution()
{
}

bool ZenithDistribution::readFile()
{
  QString filename = "plots/perdaix_cos_theta.root";
  qDebug() << "Reading file" << qPrintable(filename);
  QString hName = "cosThetaHistoValid";
  TFile* openfile = 0;
  openfile = new TFile(qPrintable(filename));
  if (openfile->IsZombie())
    return false;
  gROOT->cd();
  TH1D* zenithAcceptance = (TH1D*)(((TH1D*)openfile->Get(qPrintable(hName)))->Clone());
  double integral = zenithAcceptance->Integral("width");
  zenithAcceptance->Scale(1/integral);
  openfile->Close();
  delete openfile;
  openfile = 0;
  zenithAcceptance->SetStats(false);
  m_zenithAcceptance = zenithAcceptance;
  m_zenithAcceptance->SetMarkerColor(kBlue);
  m_zenithAcceptance->SetLineColor(kBlue);
  m_zenithAcceptance->SetLineWidth(2);
  return true;
}

