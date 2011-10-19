#include "RigidityMcSpectrum.hh"

#include "PostAnalysisCanvas.hh"

#include <TH1D.h>
#include <TCanvas.h>
#include <TAxis.h>

RigidityMcSpectrum::RigidityMcSpectrum(PostAnalysisCanvas* generated, PostAnalysisCanvas* reconstructed, PostAnalysisCanvas* reconstructedLow, PostAnalysisCanvas* reconstructedUp)
  : PostAnalysisPlot()
  , m_generated(0)
  , m_reconstructed(0)
{
  m_generated = new TH1D(*generated->histograms1D().at(0));
  m_generated->SetTitle(qPrintable(QString(generated->name()).remove(" canvas")));
  m_reconstructed = new TH1D(*reconstructed->histograms1D().at(0));
  m_reconstructed->SetTitle(qPrintable(QString(reconstructed->name()).remove(" canvas")));
  
  m_reconstructedLow = new TH1D(*reconstructedLow->histograms1D().at(0));
  m_reconstructedLow->SetTitle(qPrintable(QString(reconstructedLow->name()).remove(" canvas")));
  
  m_reconstructedUp = new TH1D(*reconstructedUp->histograms1D().at(0));
  m_reconstructedUp->SetTitle(qPrintable(QString(reconstructedUp->name()).remove(" canvas")));
}

RigidityMcSpectrum::~RigidityMcSpectrum()
{
  delete m_generated;
  delete m_reconstructed;
  delete m_reconstructedLow;
  delete m_reconstructedUp;
}

TH1D* RigidityMcSpectrum::createRecordedSpectrum()
{
  TH1D* histogram = new TH1D(*m_reconstructed);
  histogram->SetTitle("MC recorded spectrum");
  histogram->SetMarkerColor(kBlue);
  histogram->SetLineColor(kBlue);
  histogram->GetXaxis()->SetTitle("rigidity / GV");
  histogram->GetYaxis()->SetTitle("entries");
  return histogram;
}

TH1D* RigidityMcSpectrum::createRecordedSpectrumLow()
{
  TH1D* histogram = new TH1D(*m_reconstructedLow);
  histogram->SetTitle("MC recorded low spectrum");
  histogram->SetMarkerColor(kBlue);
  histogram->SetLineColor(kBlue);
  histogram->GetXaxis()->SetTitle("rigidity / GV");
  histogram->GetYaxis()->SetTitle("entries");
  return histogram;
}

TH1D* RigidityMcSpectrum::createRecordedSpectrumUp()
{
  TH1D* histogram = new TH1D(*m_reconstructedUp);
  histogram->SetTitle("MC recorded up spectrum");
  histogram->SetMarkerColor(kBlue);
  histogram->SetLineColor(kBlue);
  histogram->GetXaxis()->SetTitle("rigidity / GV");
  histogram->GetYaxis()->SetTitle("entries");
  return histogram;
}

TH1D* RigidityMcSpectrum::createGeneratedSpectrum()
{
  TH1D* histogram = new TH1D(*m_generated);
  histogram->SetTitle("MC generated spectrum");
  histogram->SetMarkerColor(kRed);
  histogram->SetLineColor(kRed);
  histogram->GetXaxis()->SetTitle("rigidity / GV");
  histogram->GetYaxis()->SetTitle("entries");
  return histogram;
}
