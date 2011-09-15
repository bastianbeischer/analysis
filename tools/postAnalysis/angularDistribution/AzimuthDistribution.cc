#include "AzimuthDistribution.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TAxis.h>
#include <TFile.h>
#include <TROOT.h>

#include <cmath>

#include <QDebug>

AzimuthDistribution::AzimuthDistribution(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_azimuthAcceptance(0)
{
  TH1D* histogram = new TH1D(*canvas->histograms1D().at(0));

  double integral = histogram->Integral("width");
  histogram->Scale(1./integral);

  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);

  if (readFile())
    addHistogram(m_azimuthAcceptance);

  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), "probability");
}

AzimuthDistribution::~AzimuthDistribution()
{
}

bool AzimuthDistribution::readFile()
{
  QString filename = "plots/azimuthAcceptance.root";
  qDebug() << "Reading file" << qPrintable(filename);
  QString hName = "phiHistoValid";
  TFile* openfile = 0;
  openfile = new TFile(qPrintable(filename));
  if (openfile->IsZombie())
    return false;
  gROOT->cd();
  TH1D* azimuthAcceptance = static_cast<TH1D*>(openfile->Get(qPrintable(hName))->Clone());
  double integral = azimuthAcceptance->Integral("width");
  azimuthAcceptance->Scale(1./integral);
  openfile->Close();
  delete openfile;
  openfile = 0;

  m_azimuthAcceptance = new TH1D("acceptance phi", "", azimuthAcceptance->GetNbinsX(), -180, 180);
  for (int i = 0; i < m_azimuthAcceptance->GetNbinsX(); ++i) {
    double azimuth = m_azimuthAcceptance->GetBinCenter(i+1);
    int bin = azimuthAcceptance->FindBin(angleMovement(azimuth));
    double content = azimuthAcceptance->GetBinContent(bin);
    m_azimuthAcceptance->SetBinContent(i, content);
  }
  delete azimuthAcceptance;
  azimuthAcceptance = 0;
  m_azimuthAcceptance->SetMarkerColor(kBlue);
  m_azimuthAcceptance->SetLineColor(kBlue);
  m_azimuthAcceptance->SetLineWidth(2);
  m_azimuthAcceptance->GetYaxis()->SetTitleOffset(2);
  return true;
}

double AzimuthDistribution::angleMovement(double azimuth)
{
  double newAzimuth = azimuth + 90;
  if (azimuth < -90)
    newAzimuth += 360;
  return newAzimuth;
}
