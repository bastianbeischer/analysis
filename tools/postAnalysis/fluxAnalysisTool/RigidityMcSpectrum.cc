#include "RigidityMcSpectrum.hh"

#include "PostAnalysisCanvas.hh"

#include <TH1D.h>
#include <TCanvas.h>
#include <TAxis.h>

RigidityMcSpectrum::RigidityMcSpectrum(PostAnalysisCanvas* generated, PostAnalysisCanvas* reconstructed)
  : PostAnalysisPlot()
  , m_generated(0)
  , m_reconstructed(0)
{
  m_generated = new TH1D(*generated->histograms1D().at(0));
  m_generated->SetTitle(qPrintable(QString(generated->name()).remove(" canvas")));
  m_reconstructed = new TH1D(*reconstructed->histograms1D().at(0));
  m_reconstructed->SetTitle(qPrintable(QString(reconstructed->name()).remove(" canvas")));
}

RigidityMcSpectrum::~RigidityMcSpectrum()
{
}

TH1D* RigidityMcSpectrum::createRecordedSpectrum()
{
  TH1D* histogram = new TH1D(*m_reconstructed);
  histogram->SetTitle("MC generated spectrum");
  histogram->SetMarkerColor(kBlue);
  histogram->SetLineColor(kBlue);
  histogram->GetXaxis()->SetTitle("rigidity / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  return histogram;
}

TH1D* RigidityMcSpectrum::createGeneratedSpectrum()
{
  TH1D* histogram = new TH1D(*m_generated);
  histogram->SetTitle("MC recorded spectrum");
  histogram->SetMarkerColor(kRed);
  histogram->SetLineColor(kRed);
  histogram->GetXaxis()->SetTitle("rigidity / GeV");
  histogram->GetYaxis()->SetTitle("entries");
  return histogram;
}

void RigidityMcSpectrum::scaleToBinWidth(TH1D* histogram)
{
  if (!histogram->GetSumw2())
    histogram->Sumw2();
  histogram->Scale(1., "width");
}